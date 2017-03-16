/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
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

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_rtex.h"
#include "ext/standard/php_string.h"

/* If you declare any globals in php_rtex.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(rtex)
*/

/* True global resources - no need for thread safety here */
static int le_rtex;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("rtex.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_rtex_globals, rtex_globals)
    STD_PHP_INI_ENTRY("rtex.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_rtex_globals, rtex_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_rtex_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_rtex_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "rtex", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(rtex_class_rename)
{
	zend_string *old;
	zend_string *old_lower, *new_lower;
	char *new;
	zend_ulong newl;
	zval *zce;
	zend_class_entry *ce;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "Ss", &old, &new, &newl) == FAILURE) {
		return;
	}

	if (ZSTR_LEN(old) == 0 || newl == 0) {
		RETURN_FALSE;
	}

	old_lower = php_string_tolower(old);

	if (new[0] == '\\') {
		new_lower = zend_string_alloc(newl-1, 1);
		zend_str_tolower_copy(ZSTR_VAL(new_lower), new+1, newl-1);
	} else {
		new_lower = zend_string_alloc(newl, 1);
		zend_str_tolower_copy(ZSTR_VAL(new_lower), new, newl);
	}

	if (zend_hash_find(EG(class_table), new_lower) ||
			zend_hash_find(CG(class_table), new_lower)) {
		zend_error(E_ERROR, "Class name %s already in use", new);
		RETURN_FALSE;
	}

	zend_assert_valid_class_name(new_lower);

	ce = zend_hash_find_ptr(CG(class_table), old_lower);
	if (ce) {
		ce->refcount++;
		zend_string_release(ce->name);
		ce->name = zend_string_copy(new_lower);
		zend_hash_del(CG(class_table), old_lower);
		zend_hash_add_ptr(CG(class_table), new_lower, ce);
	}

	ce = zend_hash_find_ptr(EG(class_table), old_lower);
	if (ce) {
		ce->refcount++;
		zend_string_release(ce->name);
		ce->name = zend_string_copy(new_lower);
		zend_hash_del(EG(class_table), old_lower);
		zend_hash_add_ptr(EG(class_table), new_lower, ce);
	}

	zend_string_release(old_lower);
	zend_string_release(new_lower);

	RETURN_TRUE;
}
/* {{{ php_rtex_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_rtex_init_globals(zend_rtex_globals *rtex_globals)
{
	rtex_globals->global_value = 0;
	rtex_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(rtex)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(rtex)
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
PHP_RINIT_FUNCTION(rtex)
{
#if defined(COMPILE_DL_RTEX) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(rtex)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(rtex)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "rtex support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */
ZEND_BEGIN_ARG_INFO_EX(arginfo_rtex_class_rename, 0, 0, 2)
	ZEND_ARG_INFO(0, old_name)
	ZEND_ARG_INFO(0, new_name)
ZEND_END_ARG_INFO()

/* {{{ rtex_functions[]
 *
 * Every user visible function must have an entry in rtex_functions[].
 */
const zend_function_entry rtex_functions[] = {
	PHP_FE(rtex_class_rename,       arginfo_rtex_class_rename)
	PHP_FE_END	/* Must be the last line in rtex_functions[] */
};
/* }}} */

/* {{{ rtex_module_entry
 */
zend_module_entry rtex_module_entry = {
	STANDARD_MODULE_HEADER,
	"rtex",
	rtex_functions,
	PHP_MINIT(rtex),
	PHP_MSHUTDOWN(rtex),
	PHP_RINIT(rtex),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(rtex),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(rtex),
	PHP_RTEX_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RTEX
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(rtex)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
