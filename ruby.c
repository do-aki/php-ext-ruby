/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 310447 2011-04-23 21:14:10Z bjori $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ruby.h"

/* If you declare any globals in php_ruby.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(ruby)
*/

/* True global resources - no need for thread safety here */
static int le_ruby;

/* {{{ ruby_functions[]
 *
 * Every user visible function must have an entry in ruby_functions[].
 */
const zend_function_entry ruby_functions[] = {
    PHP_FE(ruby_version, NULL)
    PHP_FE(ruby_eval, NULL)
    PHP_FE(ruby_require, NULL)
	PHP_FE_END	/* Must be the last line in ruby_functions[] */
};
/* }}} */

/* {{{ ruby_module_entry
 */
zend_module_entry ruby_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"ruby",
	ruby_functions,
	PHP_MINIT(ruby),
	PHP_MSHUTDOWN(ruby),
	PHP_RINIT(ruby),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(ruby),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(ruby),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RUBY
ZEND_GET_MODULE(ruby)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("ruby.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_ruby_globals, ruby_globals)
    STD_PHP_INI_ENTRY("ruby.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_ruby_globals, ruby_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_ruby_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_ruby_init_globals(zend_ruby_globals *ruby_globals)
{
	ruby_globals->global_value = 0;
	ruby_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ruby)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/

	ruby_init();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ruby)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ruby)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ruby)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ruby)
{
    VALUE ruby_version = rb_eval_string("RUBY_VERSION");

	php_info_print_table_start();
	php_info_print_table_header(2, "ruby support", "enabled");
    php_info_print_table_row(2, "ruby version", StringValuePtr(ruby_version));
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto mixed ruby_version() */
PHP_FUNCTION(ruby_version)
{
    VALUE version;
    version = rb_const_get(rb_mKernel, rb_intern("RUBY_VERSION"));

    RETURN_STRING(StringValuePtr(version), 1);
}
/* }}}*/

/* {{{ proto mixed ruby_eval(string code)
   eval ruby code */
PHP_FUNCTION(ruby_eval)
{
	char *code = NULL;
	int code_len;
    int state;
	VALUE value;
 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &code, &code_len) == FAILURE) {
		return;
	}

	value = rb_eval_string_protect(code, &state);
    if (0 != state) {
      php_error_docref(NULL TSRMLS_CC, E_WARNING, "eval failed.");
    }
    php_ruby_value_to_zval(value, return_value);
}
/* }}} */

/* {{{ proto mixed ruby_require(string file) */
PHP_FUNCTION(ruby_require)
{
	char *_filename;
    int filename_len;
	char *filename;
	char resolved_path_buff[MAXPATHLEN];
	FILE *file;
    VALUE retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &_filename, &filename_len) == FAILURE) {
		return;
	}

    if (IS_ABSOLUTE_PATH(_filename, filename_len)) {
        filename = estrdup(_filename);
	} else {
		char *ret = NULL;
		char cwd[MAXPATHLEN];
#if HAVE_GETCWD
		ret = VCWD_GETCWD(cwd, MAXPATHLEN);
#elif HAVE_GETWD
		ret = VCWD_GETWD(cwd);
#endif
		filename = ecalloc(filename_len + strlen(cwd) + 1, sizeof(char));
		strcat(filename, cwd);
		strcat(filename, "/");
		strcat(filename, _filename);
	}

	if (!VCWD_REALPATH(filename, resolved_path_buff)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "realpath failed. '%s'", filename);
		efree(filename);
		return;
	}
	efree(filename);

    file = fopen(resolved_path_buff, "r");
	if (!file) {
        php_error_docref(NULL TSRMLS_CC, E_WARNING, "can not open file. [%s]", resolved_path_buff);
		return;
	}
	fclose(file);

	retval = rb_require(resolved_path_buff);
    php_ruby_value_to_zval(retval, return_value);
}
/* }}}*/

static int hash_to_zval(VALUE key, VALUE value, VALUE zval_array) { /* {{{ */
  zval *v;
  zval *ary;

  if (key == Qundef) {
    return ST_CONTINUE;
  }

  ary = (zval*)zval_array;
  MAKE_STD_ZVAL(v);
  php_ruby_value_to_zval(value, v);

  if (Qtrue == key) {
    zend_hash_index_update(Z_ARRVAL_P(ary), 1, &v, sizeof(zval *), NULL);
    return ST_CONTINUE;
  }

  if (Qfalse == key || Qnil == key) {
    zend_hash_index_update(Z_ARRVAL_P(ary), 0, &v, sizeof(zval *), NULL);
    return ST_CONTINUE;
  }

  if (T_FIXNUM == TYPE(key)) {
    int idx = FIX2INT(key);
    zend_hash_index_update(Z_ARRVAL_P(ary), idx, &v, sizeof(zval *), NULL);
    return ST_CONTINUE;
  }

  switch (TYPE(key)) {
    case T_BIGNUM:
      key = rb_big2str(key, 10);
      break;
    case T_SYMBOL:
      key = rb_sym_to_s(key);
      break;
    case T_STRING:
      key = rb_string_value(&key);
      break;
    default:
	  key = rb_new_str2("__inconvertible__");
  }

  zend_symtable_update(Z_ARRVAL_P(ary), RSTRING_PTR(key), RSTRING_LEN(key) + 1, &v, sizeof(zval *), NULL);

  return ST_CONTINUE;
}
/* }}} */

PHP_RUBY_API void php_ruby_value_to_zval(VALUE value, zval *val) { /* {{{ */

	switch(TYPE(value)) {
		case T_FALSE:
			ZVAL_FALSE(val);
            return;
		case T_TRUE:
			ZVAL_TRUE(val);
            return;
		case T_UNDEF:
		case T_NIL:
			ZVAL_NULL(val);
            return;
		case T_FIXNUM:
		case T_BIGNUM:
			ZVAL_LONG(val, NUM2LONG(value));
            return;
		case T_FLOAT:
			ZVAL_DOUBLE(val, RFLOAT_VALUE(value));
            return;
        case T_ARRAY: {
            array_init(val);
			int i, len = RARRAY_LEN(value);
            for (i = 0; i < len; i++) {
              zval *tmp;
              VALUE v = RARRAY_PTR(value)[i];
              MAKE_STD_ZVAL(tmp);
              php_ruby_value_to_zval(v, tmp);
              zend_hash_next_index_insert(Z_ARRVAL_P(val), &tmp, sizeof(zval *), NULL);
            }
            return;
        }
		case T_HASH:
			array_init(val);
            rb_hash_foreach(value, hash_to_zval, (VALUE)val);
			return;
		case T_STRING:
		default:
			ZVAL_STRING(val, StringValuePtr(value), 1);
	}
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
