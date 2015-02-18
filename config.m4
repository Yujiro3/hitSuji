dnl $Id$
dnl config.m4 for extension hitSuji

PHP_ARG_ENABLE(hitSuji, whether to enable hitSuji support,
[  --enable-hitsuji           Enable hitSuji support])

if test "$PHP_HITSUJI" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-hitsuji -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/hitsuji.h"  # you most likely want to change this
  dnl if test -r $PHP_HITSUJI/$SEARCH_FOR; then # path given as parameter
  dnl   HITSUJI_DIR=$PHP_HITSUJI
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for hitsuji files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HITSUJI_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HITSUJI_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the hitsuji distribution])
  dnl fi

  dnl # --with-hitsuji -> add include path
  dnl PHP_ADD_INCLUDE($HITSUJI_DIR/include)

  dnl # --with-hitsuji -> check for lib and symbol presence
  dnl LIBNAME=hitsuji # you may want to change this
  dnl LIBSYMBOL=hitsuji # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HITSUJI_DIR/lib, HITSUJI_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HITSUJILIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong hitsuji lib version or lib not found])
  dnl ],[
  dnl   -L$HITSUJI_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(HITSUJI_SHARED_LIBADD)

  PHP_NEW_EXTENSION(hitsuji, php_hitsuji.c src/array.c src/callable.c src/object.c src/utility.c src/router.c src/validate.c, $ext_shared)
fi
