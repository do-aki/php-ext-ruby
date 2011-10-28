dnl $Id$
dnl config.m4 for extension ruby

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(ruby, for ruby support,
Make sure that the comment is aligned:
[  --with-ruby             Include ruby support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(ruby, whether to enable ruby support,
dnl Make sure that the comment is aligned:
dnl [  --enable-ruby           Enable ruby support])

if test "$PHP_RUBY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-ruby -> check with-path
  dnl EARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl EARCH_FOR="/include/ruby.h"  # you most likely want to change this
  dnl f test -r $PHP_RUBY/$SEARCH_FOR; then # path given as parameter
  dnl  RUBY_DIR=$PHP_RUBY
  dnl lse # search default path list
  dnl  AC_MSG_CHECKING([for ruby files in default path])
  dnl  for i in $SEARCH_PATH ; do
  dnl    if test -r $i/$SEARCH_FOR; then
  dnl      RUBY_DIR=$i
  dnl      AC_MSG_RESULT(found in $i)
  dnl    fi
  dnl  done
  dnl i
  
  if test -z "$RUBY_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the ruby distribution])
  fi

  dnl # --with-ruby -> add include path
  dnl PHP_ADD_INCLUDE($RUBY_DIR/include)
  PHP_ADD_INCLUDE(/usr/lib64/ruby/1.8/)

  dnl # --with-ruby -> check for lib and symbol presence
  dnl LIBNAME=ruby # you may want to change this
  dnl LIBSYMBOL=ruby # you most likely want to change this 
  LIBNAME=ruby
  LIBSYMBOL=ruby 

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, /usr/lib64, RUBY_SHARED_LIBADD)
    AC_DEFINE(HAVE_RUBYLIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong ruby lib version or lib not found])
  ],[
    -L/usr/lib64/lib -lruby
  ])
  
  PHP_SUBST(RUBY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ruby, ruby.c, $ext_shared)
fi
