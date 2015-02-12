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
    zval *request_ptr;
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
    /* コンフィグの初期化 */
    zend_try {
        zend_class_entry *ce = zend_fetch_class(ZEND_STRL("hitSuji\\Bootstrap"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);

        ALLOC_INIT_ZVAL(self->bootstrap);
        object_init_ex(self->bootstrap, ce);

        zend_call_method_with_1_params(
            (zval **)&self->bootstrap, 
            ce, 
            NULL, 
            "__construct", 
            NULL, 
            self->bootstrap
        );
    }
    zend_end_try();

    /* ルーターの初期化 */
    ALLOC_INIT_ZVAL(self->router);
    object_init_ex(self->router, hitsuji_router_ce);
    CALL_METHOD(HSJRouter, __construct, NULL, self->router);

    /* リクエストの初期化 */
    ALLOC_INIT_ZVAL(self->request);
    object_init_ex(self->request, hitsuji_request_ce);
    CALL_METHOD(HSJRequest, __construct, NULL, self->request);

    /* ビューの初期化 */
    ALLOC_INIT_ZVAL(self->view);
    object_init_ex(self->view, hitsuji_view_ce);
    CALL_METHOD(HSJView, __construct, NULL, self->view);

    /* アプリケーションの初期化 */
    ALLOC_INIT_ZVAL(self->delegate);
    object_init_ex(self->delegate, hitsuji_delegate_ce);
    CALL_METHOD(HSJDelegate, __construct, NULL, self->delegate);

    /* リクエスト用 */
    ALLOC_INIT_ZVAL(self->request_ptr);
    array_init(self->request_ptr);
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
    if (NULL != self->router) {
        zval_ptr_dtor(&self->router);
    }

    /* Bootstrapオブジェクトの破棄 */
    if (NULL != self->bootstrap) {
        zval_ptr_dtor(&self->bootstrap);
    }

    /* リクエスト配列の破棄 */
    if (NULL != self->request_ptr) {
        zval_ptr_dtor(&self->request_ptr);
    }

    if (NULL != hitsuji_object_ptr) {
        zval_ptr_dtor(&hitsuji_object_ptr);
    }
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
    ALLOC_INIT_ZVAL(hitsuji_object_ptr);
    object_init_ex(hitsuji_object_ptr, hitsuji_ce);

    zend_call_method_with_0_params(
        (zval **)&hitsuji_object_ptr, 
        Z_OBJCE_P(hitsuji_object_ptr), 
        NULL, 
        "__construct", 
        NULL
    );
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
    char *key = NULL, *dir;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    dir = getDir(key);

    RETURN_STRING(dir, 1);
}

/**
 * hitSujiクラス関数:pattern
 *
 * @param  string $key
 * @return string
 */
PHP_METHOD(hitSuji, pattern)
{
    char *key = NULL, *pattern;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &key, &key_len) == FAILURE) {
        RETURN_FALSE;
    }
    pattern = getPattern(key);

    RETURN_STRING(pattern, 1);
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
    zval *seed, retval, *retval_ptr;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &seed) == FAILURE &&
        IS_STRING != Z_TYPE_P(seed)) 
    {
        RETURN_FALSE;
    }
    retval_ptr = makeNonce(&retval, seed);

    RETVAL_STRINGL(Z_STRVAL_P(retval_ptr), Z_STRLEN_P(retval_ptr), 1);
    zval_ptr_dtor(&retval_ptr);
}

#   endif       /* #ifndef HAVE_HITSUJI_CLASS_HITSUJI */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_HITSUJI_H */
