dnl $Id$
dnl config.m4 for extension rtex

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(rtex, for rtex support,
Make sure that the comment is aligned:
[  --with-rtex             Include rtex support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(rtex, whether to enable rtex support,
Make sure that the comment is aligned:
[  --enable-rtex           Enable rtex support])

if test "$PHP_RTEX" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-rtex -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/rtex.h"  # you most likely want to change this
  dnl if test -r $PHP_RTEX/$SEARCH_FOR; then # path given as parameter
  dnl   RTEX_DIR=$PHP_RTEX
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for rtex files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       RTEX_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$RTEX_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the rtex distribution])
  dnl fi

  dnl # --with-rtex -> add include path
  dnl PHP_ADD_INCLUDE($RTEX_DIR/include)

  dnl # --with-rtex -> check for lib and symbol presence
  dnl LIBNAME=rtex # you may want to change this
  dnl LIBSYMBOL=rtex # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $RTEX_DIR/$PHP_LIBDIR, RTEX_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_RTEXLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong rtex lib version or lib not found])
  dnl ],[
  dnl   -L$RTEX_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(RTEX_SHARED_LIBADD)

  PHP_NEW_EXTENSION(rtex, rtex.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
