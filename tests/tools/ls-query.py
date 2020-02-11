#!/usr/bin/env python3
# coding: utf-8
"""
Query Livestatus over TCP socket.

Read a Livestatus query from stdin and send it to Livestatus over TCP. The
response is printed on stdout.
"""
from __future__ import print_function, unicode_literals

import argparse
import contextlib
import logging
import socket
import sys
import time

BUFSIZE = 4096
DEFAULT_TEST_TIMEOUT = 0.5


class LivestatusError(Exception):
    """Errors raised while communicating with Livestatus."""


def query_livestatus(address, port, query):
    """Send query to Livestatus and return the reponse as str."""
    chunks = []
    with contextlib.closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as sock:
        sock.settimeout(1.0)
        try:
            sock.connect((address, port))
            sock.sendall(query.encode("utf-8"))
            sock.shutdown(socket.SHUT_WR)

            while True:
                buf = sock.recv(BUFSIZE)
                if not buf:
                    break
                logging.debug("Received chunk of %d bytes", len(buf))
                chunks.append(buf)
        except (socket.timeout, socket.error) as e:
            logging.error("%s", e)
            logging.debug("Got socket exception", exc_info=1)
            raise LivestatusError("Failed while communicating with Livestatus")

    response = b"".join(chunks)
    logging.debug("Received a total of %d bytes", len(response))
    return response.decode("utf-8")


def connect_livestatus(address, port, timeout):
    """Test connection to Livestatus. Return True on success, False on error."""
    with contextlib.closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as sock:
        logging.info("Connect to Livestatus at %s:%s ...", address, port)
        sock.settimeout(timeout)
        try:
            sock.connect((address, port))
        except (socket.timeout, socket.error) as e:
            logging.info("Failed to connect to %s:%s: %s", address, port, e)
            return False
        else:
            logging.info("Successfully opened connection to Livestatus")
            return True


def main():
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("address", help="Hostname/IP-address")
    parser.add_argument("port", type=int, help="TCP port")
    parser.add_argument(
        "--debug",
        action="store_const",
        dest="log_level",
        const=logging.DEBUG,
        default=logging.WARNING,
        help="Turn on debug logging",
    )
    parser.add_argument(
        "--test-connect",
        action="store_true",
        help="Test connecting to the socket, exit status is 0 for success, 1 for failure",
    )
    parser.add_argument(
        "--test-timeout",
        type=float,
        default=DEFAULT_TEST_TIMEOUT,
        metavar="seconds",
        help="Timeout used for '--test-connect'. Default is %(default).1f s.",
    )
    args = parser.parse_args()

    logging.basicConfig(
        level=args.log_level, format="%(asctime)s %(levelname)s: %(message)s"
    )

    logging.debug("Python version: %r", sys.version)
    if args.test_connect:
        if connect_livestatus(args.address, args.port, args.test_timeout):
            return
        sys.exit(1)

    query = sys.stdin.read()
    logging.debug("Query Livestatus at %s:%s with: %r", args.address, args.port, query)
    start = time.time()
    try:
        response = query_livestatus(args.address, args.port, query)
        logging.debug("Query finished, time elapsed: %.2f seconds", time.time() - start)
        print(response.strip())
    except LivestatusError as e:
        logging.critical("%s", e)
        sys.exit(1)


if __name__ == "__main__":
    main()
