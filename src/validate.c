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

#ifndef HAVE_HITSUJI_VALIDATE
#define HAVE_HITSUJI_VALIDATE

#include "php.h"
#include "ext/standard/url.h"
#include "ext/standard/php_smart_str.h"
#include "ext/pcre/php_pcre.h"
#include "zend_interfaces.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_VALIDATE_H
#   include "src/validate.h"
#endif

/**
 * 数値判定
 *
 * @access public
 * @param  zval    *input  入力値
 * @return boolean
 */
int number_verify(zval *input)
{
    int result = 1;

    if (IS_LONG == Z_TYPE_P(input) || IS_DOUBLE == Z_TYPE_P(input)) {
        return result;
    } else if (IS_STRING == Z_TYPE_P(input)) {
        zend_uchar type = is_numeric_string(Z_STRVAL_P(input), Z_STRLEN_P(input), NULL, NULL, 0);
        if (IS_LONG != type && IS_DOUBLE != type) {
            result = 0;
        }
    } else if (IS_ARRAY == Z_TYPE_P(input)) {
        HashPosition pos;
        zval **target;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos)
        ) {
            number_verify(*target);
            if (!number_verify(*target)) {
                return result = 0;
            }
        }
    } else {
        result = 0;
    }

    return result;
}

/**
 * 文字列判定
 *
 * @access public
 * @param  zval    *input  入力値
 * @return boolean
 */
int string_verify(zval *input)
{
    int result = 1;

    if (IS_STRING == Z_TYPE_P(input)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            pcre = pcre_get_compiled_regex_cache(HITSUJI_G(string_pattern), strlen(HITSUJI_G(string_pattern)) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(input), Z_STRLEN_P(input),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                result = 0;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(input)) {
        HashPosition pos;
        zval **target;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos)
        ) {
            if (!string_verify(*target)) {
                return result = 0;
            }
        }
    } else {
        result = 0;
    }
    return result;
}

/**
 * メールアドレス判定
 *
 * @access public
 * @param  zval    *input  入力値
 * @return boolean
 */
int email_verify(zval *input)
{
    int result = 1;

    if (IS_STRING == Z_TYPE_P(input)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            pcre = pcre_get_compiled_regex_cache(HITSUJI_G(email_pattern), strlen(HITSUJI_G(email_pattern)) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(input), Z_STRLEN_P(input),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                result = 0;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(input)) {
        HashPosition pos;
        zval **target;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos)
        ) {
            if (!email_verify(*target)) {
                return result = 0;
            }
        }
    } else {
        result = 0;
    }
    return result;
}

/**
 * URL判定
 *
 * @access public
 * @param  zval    *input  入力値
 * @return boolean
 */
int url_verify(zval *input)
{
    int result = 1;

    if (IS_STRING == Z_TYPE_P(input)) {
        php_url *resource = php_url_parse_ex(Z_STRVAL_P(input), Z_STRLEN_P(input));
        if (resource == NULL) {
            result = 0;
        }
        if (resource->host == NULL) {
            result = 0;
        }
        php_url_free(resource);
    } else if (IS_ARRAY == Z_TYPE_P(input)) {
        HashPosition pos;
        zval **target;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos)
        ) {
            if (!url_verify(*target)) {
                return result = 0;
            }
        }
    } else {
        result = 0;
    }
    return result;
}

/**
 * 日付判定
 *
 * @access public
 * @param  zval    *input  入力値
 * @return boolean
 */
int date_verify(zval *input)
{
    int result = 1;

    if (IS_STRING == Z_TYPE_P(input)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            pcre = pcre_get_compiled_regex_cache(HITSUJI_G(date_pattern), strlen(HITSUJI_G(date_pattern)) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(input), Z_STRLEN_P(input),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                result = 0;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(input)) {
        HashPosition pos;
        zval **target;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos)
        ) {
            if (!date_verify(*target)) {
                return result = 0;
            }
        }
    } else {
        result = 0;
    }
    return result;
}

/**
 * 日時判定
 *
 * @access public
 * @param  zval    *input  入力値
 * @return boolean
 */
int datetime_verify(zval *input)
{
    int result = 1;

    if (IS_STRING == Z_TYPE_P(input)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            pcre = pcre_get_compiled_regex_cache(HITSUJI_G(datetime_pattern), strlen(HITSUJI_G(datetime_pattern)) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(input), Z_STRLEN_P(input),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                result = 0;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(input)) {
        HashPosition pos;
        zval **target;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos)
        ) {
            if (!datetime_verify(*target)) {
                return result = 0;
            }
        }
    } else {
        result = 0;
    }
    return result;
}

/**
 * 正規表現判定
 *
 * @access public
 * @param  zval   *input  入力値
 * @param  string pattern 正規表現パターン
 * @return boolean
 */
int regex_verify(zval *input, char *pattern)
{
    int result = 1;

    if (IS_STRING == Z_TYPE_P(input)) {
        zend_try {
            zval zmatch = {0};
            pcre_cache_entry *pcre = NULL;
            pcre = pcre_get_compiled_regex_cache(pattern, strlen(pattern) TSRMLS_CC);

            if (pcre) {
                php_pcre_match_impl(
                    pcre,
                    Z_STRVAL_P(input), Z_STRLEN_P(input),
                    &zmatch, NULL, 
                    0, 0, 0, 0 TSRMLS_CC
                );
            }

            if (!zend_is_true(&zmatch)) {
                result = 0;
            }
        }
        zend_end_try();
    } else if (IS_ARRAY == Z_TYPE_P(input)) {
        HashPosition pos;
        zval **target;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void **)&target, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos)
        ) {
            if (!regex_verify(*target, pattern)) {
                return result = 0;
            }
        }
    } else {
        result = 0;
    }
    return result;
}

/**
 * Nonce値の正当性を確認
 *
 * @access public
 * @param const char *seed
 * @return int
 */
int hitsuji_nonce_verify(const char *seed)
{
    int reslut = 0;
    char *nonce;
    zval *znonce = NULL, zvalue;

    if (!getRequestValue(&zvalue, "nonce", "request")) {
        return reslut;
    }

    if (strlen(HITSUJI_G(nonce_function))) {
        zval zseed;

        ZVAL_STRING(&zseed, seed, 0);
        zend_call_method_with_1_params(NULL, NULL, NULL, HITSUJI_G(nonce_function), (zval **)&znonce, &zseed);

        nonce = (char *)emalloc(Z_STRLEN_P(znonce) + 1);
        strcpy(nonce, Z_STRVAL_P(znonce));
        zval_ptr_dtor(&znonce);
    } else {
        getNonce(nonce, seed);
    }

    /* nonce値の比較 */
    if (strncasecmp(Z_STRVAL(zvalue), nonce, strlen(nonce)) == 0) {
        reslut = 1;
    }
    efree(nonce);

    return reslut;
}

/**
 * フォームからの値の正当性を確認
 *
 * @access public
 * @param const char *key
 * @param zval  *options
 * @return int
 */
int hitsuji_verify(const char *key, zval *options)
{
    zval *zvalue = NULL;
    char *type = {"string"}, *track = {"request"}, *pattern = {'\0'};
    int  require = 0, valid = 1;

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
    ALLOC_INIT_ZVAL(zvalue);
    getRequestValue(zvalue, key, track);

    if (zend_is_true(zvalue)) {
        if (IS_LONG == Z_TYPE_P(zvalue)) {
            add_assoc_long(HITSUJI_G(vars), key, Z_LVAL_P(zvalue));
        } else if (IS_DOUBLE == Z_TYPE_P(zvalue)) {
            add_assoc_double(HITSUJI_G(vars), key, Z_DVAL_P(zvalue));
        } else if (IS_STRING == Z_TYPE_P(zvalue)) {
            add_assoc_string(HITSUJI_G(vars), key, Z_STRVAL_P(zvalue), 1);
        } else {
            add_assoc_zval(HITSUJI_G(vars), key, zvalue);
        }

        /* バリデーション */
        if (strncasecmp(type, "number", 6) == 0) {
            valid = number_verify(zvalue);
        } else if (strncasecmp(type, "string", 6) == 0) {
            valid = string_verify(zvalue);
        } else if (strncasecmp(type, "email", 5) == 0) {
            valid = email_verify(zvalue);
        } else if (strncasecmp(type, "url", 3) == 0) {
            valid = url_verify(zvalue);
        } else if (strncasecmp(type, "date", 4) == 0) {
            valid = date_verify(zvalue);
        } else if (strncasecmp(type, "datetime", 8) == 0) {
            valid = datetime_verify(zvalue);
        } else if (strncasecmp(type, "regex", 5) == 0) {
            valid = regex_verify(zvalue, pattern);
        }
    } else {
        valid = (require) ? 0 : 1;
    }
    add_assoc_bool(HITSUJI_G(checks), key, valid);

    if (NULL != zvalue) {
        zval_ptr_dtor(&zvalue);
    }
    return valid;
}


/**
 * フォームからの値の正当性を確認
 *
 * @access public
 * @param zval  *array
 * @return void
 */
int hitsuji_verifies(zval *array)
{
    HashPosition pos;
    zval **option;
    int  valid = 1;

    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
         zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&option, &pos) == SUCCESS;
         zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {
        char *key;
        uint key_len;
        ulong num_key;

        zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &key, &key_len, &num_key, 0, &pos);

        if (!hitsuji_verify(key, *option)) {
            valid = 0;
        }
    }

    return valid;
}

#endif      // #ifndef HAVE_HITSUJI_VALIDATE

