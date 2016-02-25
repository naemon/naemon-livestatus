# ===========================================================================
#       http://www.gnu.org/software/autoconf-archive/ax_check_icu.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_CHECK_ICU(version, action-if, action-if-not)
#
# DESCRIPTION
#
#   Defines ICU_LIBS, ICU_CFLAGS, ICU_CXXFLAGS. See icu-config(1) man page.
#
# LICENSE
#
#   Copyright (c) 2008 Akos Maroy <darkeye@tyrell.hu>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 6

AU_ALIAS([AC_CHECK_ICU], [AX_CHECK_ICU])
AC_DEFUN([AX_CHECK_ICU], [
  succeeded=no

  if test -z "$ICU_CONFIG"; then
    AC_PATH_PROG(ICU_CONFIG, icu-config, no)
  fi

  if test "$ICU_CONFIG" = "no" ; then
    echo "*** The icu-config script could not be found. Make sure it is"
    echo "*** in your path, and that taglib is properly installed."
    echo "*** Or see http://ibm.com/software/globalization/icu/"
  else
    ICU_VERSION=`$ICU_CONFIG --version`
    AC_MSG_CHECKING(for ICU >= $1)
        VERSION_CHECK=`expr $ICU_VERSION \>\= $1`
        if test "$VERSION_CHECK" = "1" ; then
            AC_MSG_RESULT(yes)
            succeeded=yes

            ## NOTE: This used to set ICU_CPPFLAGS, but since PKG_CHECK_MODULES
            ## sets ICU_CFLAGS, it's easier to just override the proper var here.
            ## We don't necessarily want to use the ICU_CFLAGS/ICU_CXXFLAGS
            ## set by icu-config since they may potentially include things
            ## like -ansi, and -pedantic, which causes the build to bust since
            ## we're not ansi enough, and neither is libnaemon. Thus, the calls
            ## to both "$ICU_CONFIG --cflags" and "$ICU_CONFIG --cxxflags" have
            ## been dropped
            AC_MSG_CHECKING(ICU_CFLAGS)
            ICU_CFLAGS=`$ICU_CONFIG --cppflags`
            AC_MSG_RESULT($ICU_CFLAGS)

            AC_MSG_CHECKING(ICU_LIBS)
            ICU_LIBS=`$ICU_CONFIG --ldflags`
            AC_MSG_RESULT($ICU_LIBS)
        else
            ICU_CPPFLAGS=""
            ICU_LIBS=""
            ## If we have a custom action on failure, don't print errors, but
            ## do set a variable so people can do so.
            ifelse([$3], ,echo "can't find ICU >= $1",)
        fi

        AC_SUBST(ICU_CFLAGS)
        AC_SUBST(ICU_LIBS)
  fi

  if test $succeeded = yes; then
     ifelse([$2], , :, [$2])
  else
     ifelse([$3], , AC_MSG_ERROR([Library requirements (ICU) not met.]), [$3])
  fi
])
