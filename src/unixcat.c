// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#ifndef AF_LOCAL
#define   AF_LOCAL AF_UNIX
#endif
#ifndef PF_LOCAL
#define   PF_LOCAL PF_UNIX
#endif


/* Evaluate to actual length of the `sockaddr_un' structure.  */
#ifndef SUN_LEN
# define SUN_LEN(ptr) ((size_t) (((struct sockaddr_un *) 0)->sun_path) + strlen ((ptr)->sun_path))
#endif

int copy_data(int from, int to);

struct thread_info
{
    int from;
    int to;
    int should_shutdown;
    int terminate_on_read_eof;
};


size_t read_with_timeout(int from, char *buffer, size_t size, int us)
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(from, &fds);
    struct timeval tv;
    tv.tv_sec  = us / 1000000;
    tv.tv_usec = us % 1000000;
    int retval = select(from + 1, &fds, 0, 0, &tv);
    if (retval > 0)
        return read(from, buffer, size);
    else
        return -2;
}


void *copy_thread(void *info)
{
    signal(SIGWINCH, SIG_IGN);

    struct thread_info *ti = (struct thread_info *)info;
    int from = ti->from;
    int to = ti->to;

    char buffer[65536];
    while (1)
    {
        ssize_t r = read_with_timeout(from, buffer, sizeof(buffer), 1000000);
        if (r == -1) {
            fprintf(stderr, "Error reading from %d: %s\n", from, strerror(errno));
            break;
        }
        else if (r == 0) {
            if (ti->should_shutdown)
                shutdown(to, SHUT_WR);
            if (ti->terminate_on_read_eof) {
                exit(0);
                return NULL;
            }
            break;
        }
        else if (r == -2) {
            r = 0;
        }
        char *write_pos = buffer;
        while (r) {
            size_t w = write(to, write_pos, r);
            if (w > 0)
                r -= w;
            else if (w == 0 && r > 0) {
                fprintf(stderr, "Error: Cannot write %zu bytes to %d: %s\n", w, to, strerror(errno));
                break;
            }
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s UNIX-socket\n", argv[0]);
        exit(1);
    }

    signal(SIGWINCH, SIG_IGN);

    const char *unixpath = argv[1];
    struct stat st;

    if (0 != stat(unixpath, &st))
    {
        fprintf(stderr, "No UNIX socket %s existing\n", unixpath);
        exit(2);
    }

    int sock = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Cannot create client socket: %s\n", strerror(errno));
        exit(3);
    }

    /* Connect */
    struct sockaddr_un sockaddr;
    sockaddr.sun_family = AF_LOCAL;
    strncpy(sockaddr.sun_path, unixpath, sizeof(sockaddr.sun_path));
    if (connect(sock, (struct sockaddr *) &sockaddr, SUN_LEN(&sockaddr)))
    {
        fprintf(stderr, "Couldn't connect to UNIX-socket at %s: %s.\n", unixpath, strerror(errno));
        close(sock);
        exit(4);
    }

    struct thread_info toleft_info = { sock, 1, 0,  1 };
    struct thread_info toright_info = { 0, sock, 1, 0 };
    pthread_t toright_thread, toleft_thread;
    pthread_create(&toright_thread, 0, copy_thread, (void *)&toright_info);
    pthread_create(&toleft_thread, 0, copy_thread, (void *)&toleft_info);
    pthread_join(toleft_thread, NULL);
    pthread_join(toright_thread, NULL);

    close(sock);
    return 0;
}
