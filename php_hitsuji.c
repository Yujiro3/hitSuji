/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2011-2014 sheeps.me
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @package         hitSuji
 * @copyright       Copyright (c) 2011-2014 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

#ifndef HAVE_PHP_HITSUJI
#define HAVE_PHP_HITSUJI

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/standard/info.h"
#include "ext/standard/url.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_smart_str.h"
#include "ext/pcre/php_pcre.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_ARRAY_H
#   include "src/array.h"
#endif

#ifndef HAVE_HITSUJI_CALLABLE_H
#   include "src/callable.h"
#endif

#ifndef HAVE_HITSUJI_UTILITY_H
#   include "src/utility.h"
#endif

#ifndef HAVE_HITSUJI_ROUTER_H
#   include "src/router.h"
#endif

#ifndef HAVE_HITSUJI_VALIDATE_H
#   include "src/validate.h"
#endif

#ifndef HAVE_HITSUJI_OBJECT_H
#   include "src/object.h"
#endif

/* グローバル変数宣言 */
ZEND_DECLARE_MODULE_GLOBALS(hitsuji)

/* True global resources - no need for thread safety here */
static int le_hitsuji;

/* クラス構造体 */
zend_class_entry *hitsuji_ce = NULL;
zend_class_entry *hitsuji_view_ce = NULL;
zend_class_entry *hitsuji_exception_ce = NULL;

/**
 * モジュール情報リスト
 *
 * @var zend_module_entry モジュール情報リスト
 */
zend_module_entry hitSuji_module_entry = {
    STANDARD_MODULE_HEADER,
    "hitSuji",
    NULL,
    PHP_MINIT(hitSuji),
    PHP_MSHUTDOWN(hitSuji),
    PHP_RINIT(hitSuji),        /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(hitSuji),    /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(hitSuji),
    PHP_HITSUJI_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HITSUJI
    ZEND_GET_MODULE(hitSuji)
#endif


/**
 * 実行時設定
 *
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("hitsuji.nonce_function", "", PHP_INI_ALL, OnUpdateString, nonce_function, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.lifetime", "28800", PHP_INI_ALL, OnUpdateLong, lifetime, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.seed", "asdfj5246485902rweld", PHP_INI_ALL, OnUpdateString, seed, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.template_path", "", PHP_INI_ALL, OnUpdateString, template_path, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.page_path", "", PHP_INI_ALL, OnUpdateString, page_path, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.string_pattern", "/[^\\/]/i", PHP_INI_ALL, OnUpdateString, string_pattern, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.email_pattern", "/^(([^<>()[\\]\\\\.,;:\\s@\\\"]+(\\.[^<>()[\\]\\\\.,;:\\s@\\\"]+)*)|(\\\".+\\\"))@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\])|(([a-zA-Z\\-0-9]+\\.)+[a-zA-Z]{2,}))$/", PHP_INI_ALL, OnUpdateString, email_pattern, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.date_pattern", "/[0-9]{4}\\-?[0-9]{2}\\-?[0-9]{2}/", PHP_INI_ALL, OnUpdateString, date_pattern, zend_hitsuji_globals, hitsuji_globals)
    STD_PHP_INI_ENTRY("hitsuji.datetime_pattern", "/[0-9]{4}\\-?[0-9]{2}\\-?[0-9]{2}\\s?[0-9]{2}:?[0-9]{2}\\:?[0-9]{2}/", PHP_INI_ALL, OnUpdateString, datetime_pattern, zend_hitsuji_globals, hitsuji_globals)
PHP_INI_END()

/**
 * グローバル変数の初期化処理
 *
 * @param module
 * @return int
 */
static void php_hitsuji_init_globals(zend_hitsuji_globals *hitsuji_globals)
{
    hitsuji_globals->checks = NULL;
    hitsuji_globals->vars = NULL;
    hitsuji_globals->routes = NULL;
    hitsuji_globals->requests =NULL;
    hitsuji_globals->page = NULL;
}

/**
 * モジュールメイン初期化処理
 *
 * @param module
 * @return int
 */
PHP_MINIT_FUNCTION(hitSuji)
{
    zend_class_entry ce;

    REGISTER_INI_ENTRIES();
    ZEND_INIT_MODULE_GLOBALS(hitsuji, php_hitsuji_init_globals, NULL);

    /* hitSujiクラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji", hitsuji_class_methods);
    hitsuji_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hitsuji_ce->create_object = hitsuji_ctor;

    /* viewクラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji\\View", hitsuji_view_class_methods);
    hitsuji_view_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hitsuji_view_ce->create_object = hitsuji_view_ctor;

    /* 例外用クラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji\\Exception", NULL);
    hitsuji_exception_ce = zend_register_internal_class_ex(&ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC);
    hitsuji_exception_ce->ce_flags |= ZEND_ACC_FINAL;
    hitsuji_exception_ce->constructor->common.fn_flags |= ZEND_ACC_PROTECTED;

    return SUCCESS;
}

/**
 * モジュールメインシャットダウン処理
 *
 * @param module
 * @return int
 */
PHP_MSHUTDOWN_FUNCTION(hitSuji)
{
    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

/**
 * モジュールリムーブ処理
 *
 * @param module
 * @return int
 */
PHP_RINIT_FUNCTION(hitSuji)
{
    if (NULL == HITSUJI_G(requests)) {
        ALLOC_INIT_ZVAL(HITSUJI_G(requests));
        array_init(HITSUJI_G(requests));
    }

    return SUCCESS;
}

/**
 * モジュールリムーブ・シャットダウン処理
 *
 * @param module
 * @return int
 */
PHP_RSHUTDOWN_FUNCTION(hitSuji)
{
    if (NULL != HITSUJI_G(page)) {
        zval_ptr_dtor(&HITSUJI_G(page));
        HITSUJI_G(page) = NULL;
    }

    if (NULL != HITSUJI_G(requests)) {
        zval_ptr_dtor(&HITSUJI_G(requests));
        HITSUJI_G(requests) = NULL;
    }

    if (NULL != HITSUJI_G(routes)) {
        zval_ptr_dtor(&HITSUJI_G(routes));
        HITSUJI_G(routes) = NULL;
    }

    if (NULL != HITSUJI_G(vars)) {
        zval_ptr_dtor(&HITSUJI_G(vars));
        HITSUJI_G(vars) = NULL;
    }

    if (NULL != HITSUJI_G(checks)) {
        zval_ptr_dtor(&HITSUJI_G(checks));
        HITSUJI_G(checks) = NULL;
    }

    return SUCCESS;
}

/**
 * モジュール情報設定
 *
 * @param module
 * @return int
 */
PHP_MINFO_FUNCTION(hitSuji)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "hitSuji support", "enabled");
    php_info_print_table_row(2,    "Version",          PHP_HITSUJI_VERSION);
    php_info_print_table_end();
}

#ifndef HAVE_HITSUJI_CLASS_HITSUJI
#   include "classes/hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_VIEW
#   include "classes/view.h"
#endif

#endif      // #ifndef HAVE_PHP_HITSUJI
