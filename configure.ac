#!/usr/bin/python
# -*- encoding: utf-8; py-indent-offset: 4 -*-
# +------------------------------------------------------------------+
# |             ____ _               _        __  __ _  __           |
# |            / ___| |__   ___  ___| | __   |  \/  | |/ /           |
# |           | |   | '_ \ / _ \/ __| |/ /   | |\/| | ' /            |
# |           | |___| | | |  __/ (__|   <    | |  | | . \            |
# |            \____|_| |_|\___|\___|_|\_\___|_|  |_|_|\_\           |
# |                                                                  |
# | Copyright Mathias Kettner 2012             mk@mathias-kettner.de |
# +------------------------------------------------------------------+
#
# This file is part of Check_MK.
# The official homepage is at http://mathias-kettner.de/check_mk.
#
# check_mk is free software;  you can redistribute it and/or modify it
# under the  terms of the  GNU General Public License  as published by
# the Free Software Foundation in version 2.  check_mk is  distributed
# in the hope that it will be useful, but WITHOUT ANY WARRANTY;  with-
# out even the implied warranty of  MERCHANTABILITY  or  FITNESS FOR A
# PARTICULAR PURPOSE. See the  GNU General Public License for more de-
# ails.  You should have  received  a copy of the  GNU  General Public
# License along with GNU Make; see the file  COPYING.  If  not,  write
# to the Free Software Foundation, Inc., 51 Franklin St,  Fifth Floor,
# Boston, MA 02110-1301 USA.

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([Naemon Livestatus], [1.2.0p3], [op5-users@lists.op5.com])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
m4_ifdef([AM_PATH_CPPUNIT], [AM_PATH_CPPUNIT([1.12])], [AC_MSG_WARN([CPPUnit is not installed, it is needed to be able to run unit tests <http://cppunit.sourceforge.net/>])])
AS_IF([test "x$CPPUNIT_LIBS" = "x"],
	  [has_cppunit="no"],
	  [has_cppunit="yes"])

AM_CONDITIONAL([HAVE_CPPUNIT], [test "x$has_cppunit" = "xyes"])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
m4_ifdef([AM_PROG_AR], [AM_PROG_AR], [true])
AC_PROG_LIBTOOL
AM_CONDITIONAL([HAVE_DIET], [which diet >/dev/null 2>&1])

# Checks for libraries.
AC_CHECK_LIB(socket, socket)
AC_CHECK_LIB(socket, connect)
AC_CHECK_LIB(socket, shutdown)
PKG_CHECK_MODULES(LIBICU,
				  [icu-uc, icu-i18n],
				  [have_icu=yes],
				  [have_icu=no]
				  )
AS_IF([test "$have_icu" = "no"],
	  PKG_CHECK_MODULES(LIBICU,
						[icu],
						[have_icu=yes],
						[LIBICU_LIBS="-licui18n -licuuc -licudata"]
						))

AS_IF([test "$have_icu" = "no"],
	  AC_MSG_WARN([no pc file found for libicu, guessing libs]))

PKG_CHECK_MODULES(NAEMON, [naemon >= 0.8])

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h netdb.h netinet/in.h stdint.h stdlib.h string.h strings.h sys/socket.h sys/time.h sys/timeb.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_HEADER_TIME
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_CHECK_FUNCS([bzero gettimeofday memmove regcomp select socket strcasecmp strdup strerror strtoul])

AC_CONFIG_FILES([Makefile
                 src/Makefile
				 tests/Makefile])
AC_OUTPUT
