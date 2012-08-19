dnl $Id$
dnl config.m4 for extension ruby

PHP_ARG_ENABLE(ruby, whether to enable ruby support,
Make sure that the comment is aligned:
[  --enable-ruby           Enable ruby support])

if test "$PHP_RUBY" != "no"; then

  hdrdir=[`ruby -e 'puts RbConfig::CONFIG["rubyhdrdir"]'`]
  arch_hdrdir=[`ruby -e 'puts RbConfig::CONFIG["rubyhdrdir"] + File::SEPARATOR + RbConfig::CONFIG["arch"]'`]
  PHP_ADD_INCLUDE([$hdrdir])
  PHP_ADD_INCLUDE([$arch_hdrdir])

  PHP_SUBST(RUBY_SHARED_LIBADD)
  RUBY_SHARED_LIBADD=[`ruby -e 'puts RbConfig::CONFIG["LIBS"] +  RbConfig::CONFIG["LIBRUBYARG_STATIC"]'`]

  PHP_NEW_EXTENSION(ruby, ruby.c, $ext_shared)
fi
