dnl $Id$
dnl config.m4 for extension ruby

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(ruby, for ruby support,
dnl Make sure that the comment is aligned:
dnl [  --with-ruby             Include ruby support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(ruby, whether to enable ruby support,
dnl Make sure that the comment is aligned:
dnl [  --enable-ruby           Enable ruby support])

if test "$PHP_RUBY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-ruby -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/ruby.h"  # you most likely want to change this
  dnl if test -r $PHP_RUBY/$SEARCH_FOR; then # path given as parameter
  dnl   RUBY_DIR=$PHP_RUBY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for ruby files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       RUBY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$RUBY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the ruby distribution])
  dnl fi

  dnl # --with-ruby -> add include path
  dnl PHP_ADD_INCLUDE($RUBY_DIR/include)

  dnl # --with-ruby -> check for lib and symbol presence
  dnl LIBNAME=ruby # you may want to change this
  dnl LIBSYMBOL=ruby # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $RUBY_DIR/lib, RUBY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_RUBYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong ruby lib version or lib not found])
  dnl ],[
  dnl   -L$RUBY_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(RUBY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ruby, ruby.c, $ext_shared)
fi
