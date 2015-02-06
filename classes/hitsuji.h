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
 * @copyright       Copyright (c) 2011-2015 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

#ifndef HAVE_HITSUJI_CLASS_HITSUJI_H
#define HAVE_HITSUJI_CLASS_HITSUJI_H

/**
 * hitSujiクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    zval *bootstrap;
    zval *router;
    zval *delegate;
    zval *request;
    zval *view;
} hitsuji_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(hitSuji, __construct);
PHP_METHOD(hitSuji, __destruct);
PHP_METHOD(hitSuji, bootstrap);
PHP_METHOD(hitSuji, router);
PHP_METHOD(hitSuji, delegate);
PHP_METHOD(hitSuji, request);
PHP_METHOD(hitSuji, view);
PHP_METHOD(hitSuji, dir);
PHP_METHOD(hitSuji, pattern);
PHP_METHOD(hitSuji, makeNonce);


ZEND_BEGIN_ARG_INFO_EX(hitSuji_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(hitSuji_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

extern zend_function_entry hitsuji_class_methods[];

#else
#   ifndef HAVE_HITSUJI_CLASS_HITSUJI
#   define HAVE_HITSUJI_CLASS_HITSUJI

/**
 * クラスの実装部分
 */
zend_function_entry hitsuji_class_methods[] = {
    PHP_ME(hitSuji, __construct, hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(hitSuji, __destruct,  hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(hitSuji, bootstrap,   hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, router,      hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, delegate,    hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, request,     hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, view,        hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, dir,         hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, pattern,     hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, makeNonce,   hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * hitSujiクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(hitSuji, __construct)
{
    hitsuji_t *self;
    zend_class_entry *ce = NULL;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    self = (hitsuji_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
    hitsuji_object_ptr = getThis();

    /* コンフィグの初期化 */
    zend_try {
        zval *retval_ptr = NULL;
        zend_class_entry *ce = zend_fetch_class(ZEND_STRL("hitSuji\\Bootstrap"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);

        MAKE_STD_ZVAL(self->bootstrap);
        object_init_ex(self->bootstrap, ce);

        zend_call_method(
            (zval **)&self->bootstrap,  
            Z_OBJCE_P(self->bootstrap), 
            NULL, 
            "__construct", 
            sizeof("__construct")-1, 
            (zval **)&retval_ptr, 
            0, 
            NULL, 
            NULL TSRMLS_CC
        );
    }
    zend_end_try();

    /* ルーターの初期化 */
    ALLOC_INIT_ZVAL(self->router);
    object_init_ex(self->router, hitsuji_router_ce);
    CALL_METHOD(HSJRouter, __construct, NULL, self->router);
        zval_ptr_dtor(&self->router);

    /* アプリケーションの初期化 */
    ALLOC_INIT_ZVAL(self->delegate);
    object_init_ex(self->delegate, hitsuji_delegate_ce);
    CALL_METHOD(HSJDelegate, __construct, NULL, self->delegate);

    /* リクエストの初期化 */
    ALLOC_INIT_ZVAL(self->request);
    object_init_ex(self->request, hitsuji_request_ce);
    CALL_METHOD(HSJRequest, __construct, NULL, self->request);

    /* ビューの初期化 */
    ALLOC_INIT_ZVAL(self->view);
    object_init_ex(self->view, hitsuji_view_ce);
    CALL_METHOD(HSJView, __construct, NULL, self->view);
}

/**
 * hitSujiクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(hitSuji, __destruct)
{
    hitsuji_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* ビューオブジェクトの破棄 */
    if (NULL != self->view) {
        zval_ptr_dtor(&self->view);
    }

    /* リクエストオブジェクトの破棄 */
    if (NULL != self->request) {
        zval_ptr_dtor(&self->request);
    }

    /* コアプリケーションオブジェクトの破棄 */
    if (NULL != self->delegate) {
        zval_ptr_dtor(&self->delegate);
    }

    /* ルーターオブジェクトの破棄 */
        zval_ptr_dtor(&self->router);

    /* コンフィグオブジェクトの破棄 */
    if (NULL != self->bootstrap) {
        zval_ptr_dtor(&self->bootstrap);
    }

    hitsuji_object_ptr = NULL;
}

/**
 * hitSujiクラス関数:bootstrap
 *
 * @return object
 */
PHP_METHOD(hitSuji, bootstrap)
{
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    /* ルーターの初期化 */
    MAKE_STD_ZVAL(return_value);
    object_init_ex(return_value, hitsuji_router_ce);
    CALL_METHOD(hitSuji, __construct, NULL, return_value);
}

/**
 * hitSujiクラス関数:router
 *
 * @return object
 */
PHP_METHOD(hitSuji, router)
{
    hitsuji_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(self->router, 0, 0);
}

/**
 * hitSujiクラス関数:delegate
 *
 * @return object
 */
PHP_METHOD(hitSuji, delegate)
{
    hitsuji_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(self->delegate, 1, 0);
}

/**
 * hitSujiクラス関数:request
 *
 * @return object
 */
PHP_METHOD(hitSuji, request)
{
    hitsuji_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(self->request, 0, 0);
}

/**
 * hitSujiクラス関数:view
 *
 * @return object
 */
PHP_METHOD(hitSuji, view)
{
    hitsuji_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(self->view, 0, 0);
}

/**
 * hitSujiクラス関数:dir
 *
 * @param  string $key
 * @return string
 */
PHP_METHOD(hitSuji, dir)
{
    hitsuji_t *self;
    zval *zdirs, **zdir;
    char *key = NULL;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    zdirs = zend_read_property(Z_OBJCE_P(self->bootstrap), self->bootstrap, ZEND_STRL("dirs"), 1 TSRMLS_CC);

    if (zend_hash_find(Z_ARRVAL_P(zdirs), key, strlen(key) + 1, (void **)&zdir) == FAILURE
        || IS_STRING != Z_TYPE_PP(zdir)) {
        RETURN_FALSE;
    }

    RETURN_STRING(Z_STRVAL_PP(zdir), 1);
}

/**
 * hitSujiクラス関数:pattern
 *
 * @param  string $key
 * @return string
 */
PHP_METHOD(hitSuji, pattern)
{
    hitsuji_t *self;
    zval *zdirs, **zdir;
    char *key = NULL;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    zdirs = zend_read_property(Z_OBJCE_P(self->bootstrap), self->bootstrap, ZEND_STRL("patterns"), 1 TSRMLS_CC);

    if (zend_hash_find(Z_ARRVAL_P(zdirs), key, strlen(key) + 1, (void **)&zdir) == FAILURE
        || IS_STRING != Z_TYPE_PP(zdir)) {
        RETURN_FALSE;
    }

    RETURN_STRING(Z_STRVAL_PP(zdir), 1);
}

/**
 * hitSujiクラス関数:makeNonce
 *
 * @param  string $seed
 * @return string
 */
PHP_METHOD(hitSuji, makeNonce)
{
    hitsuji_t *self;
    zval param, *retval_ptr;
    char *seed = NULL;
    uint seed_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &seed, &seed_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    ZVAL_STRINGL(&param, seed, seed_len, 1);

    /* Config::nonce() 関数の実行 */
    zend_call_method(
        (zval **)&self->bootstrap,  
        Z_OBJCE_P(self->bootstrap), 
        NULL, 
        "nonce", 
        sizeof("nonce")-1, 
        (zval **)&retval_ptr, 
        0, 
        &param, 
        NULL TSRMLS_CC
    );

    RETURN_STRING(Z_STRVAL_P(retval_ptr), 1);
}

#   endif       /* #ifndef HAVE_HITSUJI_CLASS_HITSUJI */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_HITSUJI_H */
