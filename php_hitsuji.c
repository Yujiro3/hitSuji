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

#ifndef HAVE_HITSUJI_OBJECT_H
#   include "src/object.h"
#endif

/* If you declare any globals in php_hitsuji.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(hitSuji)
*/

/* True global resources - no need for thread safety here */
static int le_hitsuji;

/* クラス構造体 */
zend_class_entry *hitsuji_ce = NULL;
zend_class_entry *hitsuji_router_ce = NULL;
zend_class_entry *hitsuji_request_ce = NULL;
zend_class_entry *hitsuji_delegate_ce = NULL;
zend_class_entry *hitsuji_view_ce = NULL;
zend_class_entry *hitsuji_exception_ce = NULL;

zval *hitsuji_object_ptr = NULL;

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
 * モジュールメイン初期化処理
 *
 * @param module
 * @return int
 */
PHP_MINIT_FUNCTION(hitSuji)
{
    zend_class_entry ce;

    /* hitSujiクラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji", hitsuji_class_methods);
    hitsuji_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hitsuji_ce->create_object = hitsuji_ctor;
    zend_declare_property_null(hitsuji_ce, ZEND_STRL("bootstrap"), ZEND_ACC_PUBLIC TSRMLS_CC);

    /* routerクラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji\\Router", hitsuji_router_class_methods);
    hitsuji_router_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hitsuji_router_ce->create_object = hitsuji_router_ctor;
    zend_declare_property_string(hitsuji_router_ce, ZEND_STRL("page"), "", ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_null(hitsuji_router_ce,   ZEND_STRL("vars"),     ZEND_ACC_PUBLIC TSRMLS_CC);

    /* requestクラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji\\Request", hitsuji_request_class_methods);
    hitsuji_request_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hitsuji_request_ce->create_object = hitsuji_request_ctor;
    zend_declare_property_bool(hitsuji_request_ce, ZEND_STRL("valid"), 1, ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_null(hitsuji_request_ce, ZEND_STRL("vars"),     ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_null(hitsuji_request_ce, ZEND_STRL("checked"),  ZEND_ACC_PUBLIC TSRMLS_CC);

    /* delegateクラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji\\Delegate", hitsuji_delegate_class_methods);
    hitsuji_delegate_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hitsuji_delegate_ce->create_object = hitsuji_delegate_ctor;

    /* viewクラスの登録 */
    INIT_CLASS_ENTRY(ce, "hitSuji\\View", hitsuji_view_class_methods);
    hitsuji_view_ce = zend_register_internal_class(&ce TSRMLS_CC);
    hitsuji_view_ce->create_object = hitsuji_view_ctor;
    zend_declare_property_string(hitsuji_view_ce, ZEND_STRL("layout"), "", ZEND_ACC_PUBLIC TSRMLS_CC);
    zend_declare_property_null(hitsuji_view_ce,   ZEND_STRL("vars"),       ZEND_ACC_PUBLIC TSRMLS_CC);

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
    /* グローバル変数 */
    hitsuji_object_ptr = NULL;

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

#ifndef HAVE_HITSUJI_CLASS_ROUTER
#   include "classes/router.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_REQUEST
#   include "classes/request.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_VIEW
#   include "classes/view.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_DELEGATE
#   include "classes/delegate.h"
#endif

#endif      // #ifndef HAVE_PHP_HITSUJI
