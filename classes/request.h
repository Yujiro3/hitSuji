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

#ifndef HAVE_HITSUJI_CLASS_REQUEST_H
#define HAVE_HITSUJI_CLASS_REQUEST_H

/**
 * \hitSuji\Controllerクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    zval pat_string;
    zval pat_email;
    zval pat_date;
    zval pat_datetime;
} hitsuji_request_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(HSJRequest, __construct);
PHP_METHOD(HSJRequest, __destruct);
PHP_METHOD(HSJRequest, verifies);
PHP_METHOD(HSJRequest, verify);
PHP_METHOD(HSJRequest, reset);
PHP_METHOD(HSJRequest, number);
PHP_METHOD(HSJRequest, string);
PHP_METHOD(HSJRequest, email);
PHP_METHOD(HSJRequest, url);
PHP_METHOD(HSJRequest, date);
PHP_METHOD(HSJRequest, datetime);
PHP_METHOD(HSJRequest, regex);
PHP_METHOD(HSJRequest, value);


ZEND_BEGIN_ARG_INFO_EX(HSJRequest_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJRequest_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, param1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJRequest_1_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, param1)
    ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJRequest_2_param, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, param1)
    ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

extern zend_function_entry hitsuji_request_class_methods[];

#else
#   ifndef HAVE_HITSUJI_CLASS_REQUEST
#   define HAVE_HITSUJI_CLASS_REQUEST

/**
 * クラスの実装部分
 */
zend_function_entry hitsuji_request_class_methods[] = {
    PHP_ME(HSJRequest, __construct,   HSJRequest_0_param,   ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HSJRequest, __destruct,    HSJRequest_0_param,   ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HSJRequest, verifies,      HSJRequest_1_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, verify,        HSJRequest_1_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, reset,         HSJRequest_0_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, number,        HSJRequest_1_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, string,        HSJRequest_1_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, email,         HSJRequest_1_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, url,           HSJRequest_1_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, date,          HSJRequest_1_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, datetime,      HSJRequest_1_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, regex,         HSJRequest_2_param,   ZEND_ACC_PUBLIC)
    PHP_ME(HSJRequest, value,         HSJRequest_1_1_param, ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * HSJRequestクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJRequest, __construct)
{
    hitsuji_request_t *self;
    zval zkey, *zvars, *zchecked;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    self = (hitsuji_request_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    ZVAL_STRING(&zkey, "string", 1);
    CALL_METHOD1(hitSuji, pattern, &self->pat_string, hitsuji_object_ptr, &zkey);

    ZVAL_STRING(&zkey, "email", 1);
    CALL_METHOD1(hitSuji, pattern, &self->pat_email, hitsuji_object_ptr, &zkey);

    ZVAL_STRING(&zkey, "date", 1);
    CALL_METHOD1(hitSuji, pattern, &self->pat_date, hitsuji_object_ptr, &zkey);

    ZVAL_STRING(&zkey, "datetime", 1);
    CALL_METHOD1(hitSuji, pattern, &self->pat_datetime, hitsuji_object_ptr, &zkey);

    /* 変数保存用配列の初期化 */
    zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);
    array_init(zvars);

    /* 変数保存用配列の初期化 */
    zchecked = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("checked"), 1 TSRMLS_CC);
    array_init(zchecked);
}

/**
 * HSJRequestクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJRequest, __destruct)
{
    hitsuji_request_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    self = (hitsuji_request_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
}

/**
 * フォームからの値の正当性を確認
 *
 * Example:
 * <code><pre>
 * $request->verifies(array(
 *     'id'=> number,
 *     'name'=> array('string', 'require'),
 *     'email'=> array('email', 'request'),
 * ));
 * </pre></code>
 *
 * @access public
 * @param array  $rule
 * @return void
 */
PHP_METHOD(HSJRequest, verifies)
{
    zval *array;
    HashPosition pos;
    zval **option;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
        RETURN_FALSE;
    }

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
         zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&option, &pos) == SUCCESS;
         zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {
        zval zkey, retval;
        char *key;
        uint key_len;
        ulong num_key;

        zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &key, &key_len, &num_key, 0, &pos);
        ZVAL_STRINGL(&zkey, key, key_len, 1);

        CALL_METHOD2(HSJRequest, verify, &retval, getThis(), &zkey, *option);
    }

    RETURN_CHAIN();
}

/**
 * フォームからの値の正当性を確認
 *
 * Example:
 * <code><pre>
 * $options = array (
 *      'number',
 *      'get',
 *      'require',
 *      '/[^\/]/i'
 * );
 * $request->verify('id', $options);
 * </pre></code>
 *
 * @access public
 * @param string $key 
 * @param array  $options
 * @return this
 */
PHP_METHOD(HSJRequest, verify)
{
    zval zkey, ztrack, zvalue;
    zval *options, *zvars, *zchecked;
    char *key, *type, *track = {"request"}, *pattern = {'\0'};
    uint key_len;
    int  require = 0, check = 1;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &key, &key_len, &options) == FAILURE) {
        RETURN_FALSE;
    }

    /* 判定条件の取得 */
    if (IS_STRING == Z_TYPE_P(options)) {
        type = Z_STRVAL_P(options);
    } else if (IS_ARRAY == Z_TYPE_P(options)) {
        HashPosition pos;
        zval **element;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(options), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(options), (void **)&element, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(options), &pos)
        ) {
            if (strncasecmp(Z_STRVAL_PP(element), "number", 6) == 0 ||
                strncasecmp(Z_STRVAL_PP(element), "string", 6) == 0 ||
                strncasecmp(Z_STRVAL_PP(element), "email", 5) == 0 ||
                strncasecmp(Z_STRVAL_PP(element), "url", 3) == 0 ||
                strncasecmp(Z_STRVAL_PP(element), "date", 4) == 0 ||
                strncasecmp(Z_STRVAL_PP(element), "datetime", 8) == 0 ||
                strncasecmp(Z_STRVAL_PP(element), "regex", 5) == 0) 
            {
                type = Z_STRVAL_PP(element);
            } else if (strncasecmp(Z_STRVAL_PP(element), "post", 4) == 0 || 
                       strncasecmp(Z_STRVAL_PP(element), "get", 3) == 0 || 
                       strncasecmp(Z_STRVAL_PP(element), "cookie", 6) == 0 || 
                       strncasecmp(Z_STRVAL_PP(element), "server", 6) == 0|| 
                       strncasecmp(Z_STRVAL_PP(element), "request", 7) == 0) 
            {
                track = Z_STRVAL_PP(element);
            } else if (strncasecmp(Z_STRVAL_PP(element), "require", 7) == 0) {
                require = 1;
            } else {
                pattern = Z_STRVAL_PP(element);
            }
        }
    }

    /* 値の取得 */
    ZVAL_STRING(&ztrack, track, 1);
    ZVAL_STRINGL(&zkey, key, key_len, 1);
    CALL_METHOD2(HSJRequest, value, &zvalue, getThis(), &zkey, &ztrack);

    if (zend_is_true(&zvalue)) {
        zval retval;

        zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);
        if (IS_LONG == Z_TYPE(zvalue)) {
            add_assoc_long(zvars, key, Z_LVAL(zvalue));
        } else if (IS_DOUBLE == Z_TYPE(zvalue)) {
            add_assoc_double(zvars, key, Z_DVAL(zvalue));
        } else if (IS_STRING == Z_TYPE(zvalue)) {
            add_assoc_string(zvars, key, Z_STRVAL(zvalue), 1);
        } else {
            add_assoc_zval(zvars, key, &zvalue);
        }

        /* バリデーション */
        if (strncasecmp(type, "number", 6) == 0) {
            CALL_METHOD1(HSJRequest, number, &retval, getThis(), &zvalue);
        } else if (strncasecmp(type, "string", 6) == 0) {
            CALL_METHOD1(HSJRequest, string, &retval, getThis(), &zvalue);
        } else if (strncasecmp(type, "email", 5) == 0) {
            CALL_METHOD1(HSJRequest, email, &retval, getThis(), &zvalue);
        } else if (strncasecmp(type, "url", 3) == 0) {
            CALL_METHOD1(HSJRequest, url, &retval, getThis(), &zvalue);
        } else if (strncasecmp(type, "date", 4) == 0) {
            CALL_METHOD1(HSJRequest, date, &retval, getThis(), &zvalue);
        } else if (strncasecmp(type, "datetime", 8) == 0) {
            CALL_METHOD1(HSJRequest, datetime, &retval, getThis(), &zvalue);
        } else if (strncasecmp(type, "regex", 5) == 0) {
            zval zpattern;

            ZVAL_STRING(&zpattern, pattern, 1);
            CALL_METHOD2(HSJRequest, regex, &retval, getThis(), &zvalue, &zpattern);
        }
        check = zend_is_true(&retval) ? 1 : 0;
    } else {
        check = (require) ? 0 : 1;
    }

    zchecked = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("checked"), 1 TSRMLS_CC);
    add_assoc_bool(zchecked, key, check);

    if (!check) {
        zend_update_property_bool(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("valid"), 0 TSRMLS_CC);
    }

    RETURN_CHAIN();
}

/**
 * 数値判定
 *
 * @access public
 * @return this
 */
PHP_METHOD(HSJRequest, reset)
{
    zval *zvars, *zchecked;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    zend_update_property_bool(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("valid"), 1 TSRMLS_CC);

    zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);
    array_all_clean(zvars);

    zchecked = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("checked"), 1 TSRMLS_CC);
    array_all_clean(zchecked);

    RETURN_CHAIN();
}

/**
 * 数値判定
 *
 * @access public
 * @param mixed $input 入力値
 * @return boolean
 */
PHP_METHOD(HSJRequest, number)
{
    zval *param;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
        RETURN_FALSE;
    }
    ZVAL_BOOL(return_value, 1);

    if (IS_LONG == Z_TYPE_P(param) || IS_DOUBLE == Z_TYPE_P(param)) {
        RETURN_TRUE;
    } else if (IS_STRING == Z_TYPE_P(param)) {
        zend_uchar type = is_numeric_string(Z_STRVAL_P(param), Z_STRLEN_P(param), NULL, NULL, 0);
        if (IS_LONG != type && IS_DOUBLE != type) {
            ZVAL_BOOL(return_value, 0);
        }
    } else if (IS_ARRAY == Z_TYPE_P(param)) {
        HashPosition pos;
        zval **target;
        zval retval;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos)
        ) {
            CALL_METHOD1(HSJRequest, number, &retval, getThis(), *target);
            if (!zend_is_true(&retval)) {
                ZVAL_BOOL(return_value, 0);
            }
        }
    } else {
        ZVAL_BOOL(return_value, 0);
    }
}

/**
 * 文字列判定
 *
 * @access public
 * @param mixed $input 入力値
 * @return boolean
 */
PHP_METHOD(HSJRequest, string)
{
    zval *param;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
        RETURN_FALSE;
    }
    ZVAL_BOOL(return_value, 1);

    if (IS_STRING == Z_TYPE_P(param)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            hitsuji_request_t *self = (hitsuji_request_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
            pcre = pcre_get_compiled_regex_cache(Z_STRVAL(self->pat_string), Z_STRLEN(self->pat_string) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(param), Z_STRLEN_P(param),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                RETVAL_FALSE;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(param)) {
        HashPosition pos;
        zval **target;
        zval retval;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos)
        ) {
            CALL_METHOD1(HSJRequest, string, &retval, getThis(), *target);
            if (!zend_is_true(&retval)) {
                ZVAL_BOOL(return_value, 0);
            }
        }
    } else {
        ZVAL_BOOL(return_value, 0);
    }
}

/**
 * メールアドレス判定
 *
 * @access public
 * @param mixed $input 入力値
 * @return boolean
 */
PHP_METHOD(HSJRequest, email)
{
    zval *param;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
        RETURN_FALSE;
    }
    ZVAL_BOOL(return_value, 1);

    if (IS_STRING == Z_TYPE_P(param)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            hitsuji_request_t *self = (hitsuji_request_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
            pcre = pcre_get_compiled_regex_cache(Z_STRVAL(self->pat_email), Z_STRLEN(self->pat_email) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(param), Z_STRLEN_P(param),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                RETVAL_FALSE;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(param)) {
        HashPosition pos;
        zval **target;
        zval retval;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos)
        ) {
            CALL_METHOD1(HSJRequest, email, &retval, getThis(), *target);
            if (!zend_is_true(&retval)) {
                ZVAL_BOOL(return_value, 0);
            }
        }
    } else {
        ZVAL_BOOL(return_value, 0);
    }
}

/**
 * URL判定
 *
 * @access public
 * @param mixed $input 入力値
 * @return boolean
 */
PHP_METHOD(HSJRequest, url)
{
    zval *param;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
        RETURN_FALSE;
    }
    ZVAL_BOOL(return_value, 1);

    if (IS_STRING == Z_TYPE_P(param)) {
        php_url *resource = php_url_parse_ex(Z_STRVAL_P(param), Z_STRLEN_P(param));
        if (resource == NULL) {
            ZVAL_BOOL(return_value, 0);
        }
        if (resource->host == NULL) {
            ZVAL_BOOL(return_value, 0);
        }
        php_url_free(resource);
    } else if (IS_ARRAY == Z_TYPE_P(param)) {
        HashPosition pos;
        zval **target;
        zval retval;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos)
        ) {
            CALL_METHOD1(HSJRequest, url, &retval, getThis(), *target);
            if (!zend_is_true(&retval)) {
                ZVAL_BOOL(return_value, 0);
            }
        }
    } else {
        ZVAL_BOOL(return_value, 0);
    }
}

/**
 * 日付判定
 *
 * @access public
 * @param mixed $input 入力値
 * @return boolean
 */
PHP_METHOD(HSJRequest, date)
{
    zval *param;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
        RETURN_FALSE;
    }
    ZVAL_BOOL(return_value, 1);

    if (IS_STRING == Z_TYPE_P(param)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            hitsuji_request_t *self = (hitsuji_request_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
            pcre = pcre_get_compiled_regex_cache(Z_STRVAL(self->pat_date), Z_STRLEN(self->pat_date) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(param), Z_STRLEN_P(param),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                RETVAL_FALSE;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(param)) {
        HashPosition pos;
        zval **target;
        zval retval;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos)
        ) {
            CALL_METHOD1(HSJRequest, date, &retval, getThis(), *target);
            if (!zend_is_true(&retval)) {
                ZVAL_BOOL(return_value, 0);
            }
        }
    } else {
        ZVAL_BOOL(return_value, 0);
    }
}

/**
 * 日時判定
 *
 * @access public
 * @param mixed $input 入力値
 * @return boolean
 */
PHP_METHOD(HSJRequest, datetime)
{
    zval *param;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE) {
        RETURN_FALSE;
    }
    ZVAL_BOOL(return_value, 1);

    if (IS_STRING == Z_TYPE_P(param)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            hitsuji_request_t *self = (hitsuji_request_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
            pcre = pcre_get_compiled_regex_cache(Z_STRVAL(self->pat_datetime), Z_STRLEN(self->pat_datetime) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(param), Z_STRLEN_P(param),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                RETVAL_FALSE;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(param)) {
        HashPosition pos;
        zval **target;
        zval retval;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos)
        ) {
            CALL_METHOD1(HSJRequest, datetime, &retval, getThis(), *target);
            if (!zend_is_true(&retval)) {
                ZVAL_BOOL(return_value, 0);
            }
        }
    } else {
        ZVAL_BOOL(return_value, 0);
    }
}

/**
 * 正規表現判定
 *
 * @access public
 * @param mixed  $input   入力値
 * @param string $pattern 正規表現パターン
 * @return boolean
 */
PHP_METHOD(HSJRequest, regex)
{
    zval *param;
    char *pattern;
    uint pattern_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zs", &param, &pattern, &pattern_len) == FAILURE) {
        RETURN_FALSE;
    }
    ZVAL_BOOL(return_value, 1);

    if (IS_STRING == Z_TYPE_P(param)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            hitsuji_request_t *self = (hitsuji_request_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
            pcre = pcre_get_compiled_regex_cache(pattern, pattern_len TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(param), Z_STRLEN_P(param),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                RETVAL_FALSE;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(param)) {
        HashPosition pos;
        zval **target;
        zval retval, zpattern;

        ZVAL_STRINGL(&zpattern, pattern, pattern_len, 1);

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos)
        ) {
            CALL_METHOD2(HSJRequest, regex, &retval, getThis(), *target, &zpattern);
            if (!zend_is_true(&retval)) {
                ZVAL_BOOL(return_value, 0);
            }
        }
    } else {
        ZVAL_BOOL(return_value, 0);
    }
}

/**
 * 入力値の取得
 *
 * @access public
 * @param string $key  添字
 * @param string $tack 参照先テーブル
 * @return mixed
 */
PHP_METHOD(HSJRequest, value)
{
    zval **value;
    zval *array_ptr = NULL;
    char *key, *track = NULL;
    uint key_len, track_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &key, &key_len, &track, &track_len) == FAILURE) {
        RETURN_FALSE;
    }

    if (strncasecmp(track, "post", 4) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_POST];
    } else if (strncasecmp(track, "get", 3) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_GET];
    } else if (strncasecmp(track, "cookie", 6) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_COOKIE];
    } else if (strncasecmp(track, "route", 5) == 0) {
        hitsuji_t *hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);
        array_ptr = zend_read_property(Z_OBJCE_P(hitsuji->router), hitsuji->router, ZEND_STRL("vars"), 1 TSRMLS_CC);
    } else if (strncasecmp(track, "server", 6) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_SERVER];
    } else {
        zval zkey, ztrack;

        ZVAL_STRINGL(&zkey, key, key_len, 1);

        ZVAL_STRING(&ztrack, "post", 1);
        CALL_METHOD2(HSJRequest, value, return_value, getThis(), &zkey, &ztrack);
        if (zend_is_true(return_value)) {
            return;
        }

        ZVAL_STRING(&ztrack, "get", 1);
        CALL_METHOD2(HSJRequest, value, return_value, getThis(), &zkey, &ztrack);
        if (zend_is_true(return_value)) {
            return;
        }

        ZVAL_STRING(&ztrack, "route", 1);
        CALL_METHOD2(HSJRequest, value, return_value, getThis(), &zkey, &ztrack);
        if (zend_is_true(return_value)) {
            return;
        }

        array_ptr = PG(http_globals)[TRACK_VARS_COOKIE];
    }

    if (SUCCESS == zend_hash_find(Z_ARRVAL_P(array_ptr), key, strlen(key) + 1, (void **) &value)) { 
        RETURN_ZVAL(*value, 1, 0);
    }

    RETURN_FALSE;
}

#   endif       /* #ifndef HAVE_HITSUJI_CLASS_REQUEST */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_REQUEST_H */
