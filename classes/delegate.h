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

#ifndef HAVE_HITSUJI_CLASS_DELEGATE_H
#define HAVE_HITSUJI_CLASS_DELEGATE_H

/**
 * \hitSuji\Delegateクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    HashTable *actions;
    HashTable *parses;
    HashTable *quicks;
    zval *always;
    zval *done;
    zval *fail;
} hitsuji_delegate_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(HSJDelegate, __construct);
PHP_METHOD(HSJDelegate, __destruct);
PHP_METHOD(HSJDelegate, run);
PHP_METHOD(HSJDelegate, reset);
PHP_METHOD(HSJDelegate, verifyNonce);
PHP_METHOD(HSJDelegate, binds);
PHP_METHOD(HSJDelegate, action);
PHP_METHOD(HSJDelegate, quick);
PHP_METHOD(HSJDelegate, parse);
PHP_METHOD(HSJDelegate, always);
PHP_METHOD(HSJDelegate, done);
PHP_METHOD(HSJDelegate, fail);
PHP_METHOD(HSJDelegate, fireAction);
PHP_METHOD(HSJDelegate, fireQuick);
PHP_METHOD(HSJDelegate, fireParse);
PHP_METHOD(HSJDelegate, fireAlways);
PHP_METHOD(HSJDelegate, fireDone);
PHP_METHOD(HSJDelegate, fireFail);


ZEND_BEGIN_ARG_INFO_EX(HSJDelegate_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJDelegate_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, param1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJDelegate_2_param, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, param1)
    ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJDelegate_3_param, 0, ZEND_RETURN_VALUE, 3)
    ZEND_ARG_INFO(0, param1)
    ZEND_ARG_INFO(0, param2)
    ZEND_ARG_INFO(0, param3)
ZEND_END_ARG_INFO()

extern zend_function_entry hitsuji_delegate_class_methods[];

#else
#   ifndef HAVE_HITSUJI_CLASS_DELEGATE
#   define HAVE_HITSUJI_CLASS_DELEGATE

/**
 * クラスの実装部分
 */
zend_function_entry hitsuji_delegate_class_methods[] = {
    PHP_ME(HSJDelegate, __construct,  HSJDelegate_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HSJDelegate, __destruct,   HSJDelegate_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HSJDelegate, run,          HSJDelegate_0_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, reset,        HSJDelegate_0_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, verifyNonce,  HSJDelegate_0_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, binds,        HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, action,       HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, quick,        HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, parse,        HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, always,       HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, done,         HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, fail,         HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, fireAction,   HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, fireQuick,    HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, fireParse,    HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, fireAlways,   HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, fireDone,     HSJDelegate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJDelegate, fireFail,     HSJDelegate_1_param, ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * HSJDelegateクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJDelegate, __construct)
{
    hitsuji_delegate_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    ALLOC_HASHTABLE(self->actions);
    zend_hash_init(self->actions, 0, NULL, ZVAL_PTR_DTOR, 0);

    ALLOC_HASHTABLE(self->parses);
    zend_hash_init(self->parses, 0, NULL, ZVAL_PTR_DTOR, 0);

    ALLOC_HASHTABLE(self->quicks);
    zend_hash_init(self->quicks, 0, NULL, ZVAL_PTR_DTOR, 0);
}

/**
 * HSJDelegateクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJDelegate, __destruct)
{
    hitsuji_delegate_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (NULL != self->fail) {
        zval_ptr_dtor(&self->fail);
    }
    if (NULL != self->done) {
        zval_ptr_dtor(&self->done);
    }

    if (NULL != self->always) {
        zval_ptr_dtor(&self->always);
    }

    if (NULL != self->quicks) {
        zend_hash_destroy(self->quicks);
        FREE_HASHTABLE(self->quicks);
    }

    if (NULL != self->parses) {
        zend_hash_destroy(self->parses);
        FREE_HASHTABLE(self->parses);
    }

    if (NULL != self->actions) {
        zend_hash_destroy(self->actions);
        FREE_HASHTABLE(self->actions);
    }
}

/**
 * 実行処理
 *
 * @access public
 * @return object
 */
PHP_METHOD(HSJDelegate, run)
{
    hitsuji_delegate_t *self;
    hitsuji_t *hitsuji;
    zval retval, _retval, retnonce, *zvars;
    zval parsed;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
    hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    zvars = zend_read_property(Z_OBJCE_P(hitsuji->request), hitsuji->request, ZEND_STRL("vars"), 1 TSRMLS_CC);

    /* parseコールバック実行 */
    if (zend_hash_num_elements(self->parses) != 0) {
        zvars = hitsuji_calls_function_1_params(self->parses, &retval, zvars);
    }
    /* nonce値の確認 */
    CALL_METHOD(HSJDelegate, verifyNonce, &retnonce, getThis());

    if (zend_is_true(&retnonce)) {
        zval *verified = zend_read_property(Z_OBJCE_P(hitsuji->request), hitsuji->request, ZEND_STRL("valid"), 1 TSRMLS_CC);
        zval *zchecked = zend_read_property(Z_OBJCE_P(hitsuji->request), hitsuji->request, ZEND_STRL("checked"), 1 TSRMLS_CC);

        if (zend_is_true(verified)) {
            /* actionコールバック実行 */
            zvars = hitsuji_calls_function_1_params(self->actions, &_retval, zvars);

            if (array_bool_data(&zvars)) {
                hitsuji_call_function_1_params(self->done, NULL, zvars);
            } else {
                hitsuji_call_function_2_params(self->fail, NULL, zvars, zchecked);
            }
        } else {
            hitsuji_call_function_2_params(self->fail, NULL, zvars, zchecked);
        }
        zval_ptr_dtor(&zchecked);
        zval_ptr_dtor(&verified);
    } else {
        zval _retval;
        if (zend_hash_num_elements(self->quicks) == 0) {
            /* alwaysコールバック実行 */
            zvars = hitsuji_calls_function_1_params(self->always, &_retval, zvars);
        } else {
            /* actionコールバック実行 */
            zvars = hitsuji_calls_function_1_params(self->quicks, &_retval, zvars);

            if (array_bool_data(zvars)) {
                hitsuji_call_function_1_params(self->done, NULL, zvars);
            } else {
                hitsuji_call_function_1_params(self->always, NULL, zvars);
            }
        }
    }
    zval_ptr_dtor(&zvars);

    RETURN_CHAIN();
}

/**
 * 登録データの消去
 *
 * @access public
 * @return object
 */
PHP_METHOD(HSJDelegate, reset)
{
    hitsuji_delegate_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    hash_all_clean(self->actions);
    hash_all_clean(self->parses);
    hash_all_clean(self->quicks);

    RETURN_CHAIN();
}

/**
 * nonce値の確認
 *
 * @access public
 * @return boolean
 */
PHP_METHOD(HSJDelegate, verifyNonce)
{
    hitsuji_t *hitsuji;
    zval *zpage, zkey, ztrack, znonce, *znmake = NULL;
    char *nonce, *nmake;
    int  result = 0;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    /* フォームからのnonce値の取得 */
    hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);
    if (!getRequestValue(&znonce, "nonce", "request", hitsuji->request_ptr)) {
        RETURN_FALSE;
    }

    /* アクセスしているpageパスの取得 */
    zpage = zend_read_property(Z_OBJCE_P(hitsuji->router), hitsuji->router, ZEND_STRL("page"), 1 TSRMLS_CC);

    /* nonce値の生成 */
    zend_call_method_with_1_params(
        (zval **)&hitsuji->bootstrap, 
        Z_OBJCE_P(hitsuji->bootstrap), 
        NULL, 
        "nonce", 
        (zval **)&znmake, 
        zpage
    );

    /* nonce値の比較 */
    result = strncasecmp(Z_STRVAL(znonce), Z_STRVAL_P(znmake), Z_STRLEN_P(znmake));
    zval_ptr_dtor(&znmake);

    if (result != 0) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}

/**
 * インプットルール定義
 *
 * @access public
 * @param array $rule
 * @return object
 */
PHP_METHOD(HSJDelegate, binds)
{
    hitsuji_t *hitsuji;
    zval *array = NULL;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
        RETURN_FALSE;
    }

    hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);
    zend_call_method_with_0_params(
        (zval **)&hitsuji->request, 
        Z_OBJCE_P(hitsuji->request), 
        NULL, 
        "reset", 
        NULL
    );

    zend_call_method_with_1_params(
        (zval **)&hitsuji->request, 
        Z_OBJCE_P(hitsuji->request), 
        NULL, 
        "verifies", 
        NULL, 
        array
    );

    RETURN_CHAIN();
}

/**
 * アクション処理関数の登録
 *
 * @access public
 * @param function $callable
 * @return object
 */
PHP_METHOD(HSJDelegate, action)
{
    hitsuji_delegate_t *self;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (!zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        zend_throw_exception(hitsuji_exception_ce, "An argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    Z_ADDREF_P(callable);

    /* ハッシュテーブルへ登録 */
    zend_hash_next_index_insert(self->actions, (void *)&callable, sizeof(zval *), NULL);

    RETURN_CHAIN();
}

/**
 * クリック処理関数の登録
 *
 * @access public
 * @param function $callable
 * @return object
 */
PHP_METHOD(HSJDelegate, quick)
{
    hitsuji_delegate_t *self;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (!zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        zend_throw_exception(hitsuji_exception_ce, "An argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    Z_ADDREF_P(callable);

    /* ハッシュテーブルへ登録 */
    zend_hash_next_index_insert(self->quicks, (void *)&callable, sizeof(zval *), NULL);

    RETURN_CHAIN();
}

/**
 * パース処理関数の登録
 *
 * @access public
 * @param function $callable
 * @return object
 */
PHP_METHOD(HSJDelegate, parse)
{
    hitsuji_delegate_t *self;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (!zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        zend_throw_exception(hitsuji_exception_ce, "An argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
    Z_ADDREF_P(callable);

    /* ハッシュテーブルへ登録 */
    zend_hash_next_index_insert(self->parses, (void *)&callable, sizeof(zval *), NULL);

    RETURN_CHAIN();
}

/**
 * デフォルト処理の登録
 *
 * @access public
 * @param function $callable
 * @return object
 */
PHP_METHOD(HSJDelegate, always)
{
    hitsuji_delegate_t *self;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (!zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        zend_throw_exception(hitsuji_exception_ce, "An argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
    self->always = callable;
    Z_ADDREF_P(self->always);

    RETURN_CHAIN();
}

/**
 * 成功処理の登録
 *
 * @access public
 * @param function $callable
 * @return object
 */
PHP_METHOD(HSJDelegate, done)
{
    hitsuji_delegate_t *self;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (!zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        zend_throw_exception(hitsuji_exception_ce, "An argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
    self->done = callable;
    Z_ADDREF_P(self->done);

    RETURN_CHAIN();
}

/**
 * 失敗処理の登録
 *
 * @access public
 * @param function $callable
 * @return object
 */
PHP_METHOD(HSJDelegate, fail)
{
    hitsuji_delegate_t *self;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (!zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        zend_throw_exception(hitsuji_exception_ce, "An argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }
    self->fail = callable;
    Z_ADDREF_P(self->fail);

    RETURN_CHAIN();
}

/**
 * アクション処理の実行
 *
 * @access public
 * @return boolean
 */
PHP_METHOD(HSJDelegate, fireAction)
{
    hitsuji_delegate_t *self;
    zval *zvars, retval, *retval_ptr;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zvars) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    retval_ptr = hitsuji_calls_function_1_params(self->actions, &retval, zvars);

    RETVAL_ZVAL(retval_ptr, 1, 1);
    zval_ptr_dtor(&retval_ptr);
}

/**
 * クイック処理の実行
 *
 * @access public
 * @return boolean
 */
PHP_METHOD(HSJDelegate, fireQuick)
{
    hitsuji_delegate_t *self;
    zval *zvars, retval, *retval_ptr;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zvars) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    retval_ptr = hitsuji_calls_function_1_params(self->quicks, &retval, zvars);

    RETVAL_ZVAL(retval_ptr, 1, 1);
    zval_ptr_dtor(&retval_ptr);
}

/**
 * パース処理の実行
 *
 * @access public
 * @param array $data
 * @return array
 */
PHP_METHOD(HSJDelegate, fireParse)
{
    hitsuji_delegate_t *self;
    zval *zvars, retval, *retval_ptr;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zvars) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    retval_ptr = hitsuji_calls_function_1_params(self->parses, &retval, zvars);

    RETVAL_ZVAL(retval_ptr, 1, 1);
    zval_ptr_dtor(&retval_ptr);
}

/**
 * デフォルト処理の実行
 *
 * @access public
 * @param array $data
 * @return object
 */
PHP_METHOD(HSJDelegate, fireAlways)
{
    hitsuji_delegate_t *self;
    zval *zvars;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zvars) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    hitsuji_call_function_1_params(self->always, NULL, zvars);

    RETURN_TRUE;
}

/**
 * 成功処理の実行
 *
 * @access public
 * @param array $data
 * @return object
 */
PHP_METHOD(HSJDelegate, fireDone)
{
    hitsuji_delegate_t *self;
    zval *zvars;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &zvars) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    hitsuji_call_function_1_params(self->done, NULL, zvars);

    RETURN_TRUE;
}

/**
 * 失敗処理の実行
 *
 * @access public
 * @param array $data
 * @param array $checked
 * @return object
 */
PHP_METHOD(HSJDelegate, fireFail)
{
    hitsuji_delegate_t *self;
    zval *zvars, *zchecked;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz", &zvars, &zchecked) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_delegate_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    hitsuji_call_function_2_params(self->fail, NULL, zvars, zchecked);

    RETURN_TRUE;
}

#   endif       /* #ifndef HAVE_HITSUJI_CLASS_DELEGATE */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_DELEGATE_H */
