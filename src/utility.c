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

#ifndef HAVE_HITSUJI_UTILITY
#define HAVE_HITSUJI_UTILITY

#include "php.h"
#include "zend_interfaces.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_UTILITY_H
#   include "src/utility.h"
#endif

/**
 * hitSuji\Bootstrapクラス変数:dirs
 *
 * @param  const char *key
 * @return char
 */
char *getDir(const char *key)
{
    hitsuji_t *hitsuji;
    zval *zdirs, **zdir;
 
    hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);
    zdirs = zend_read_property(Z_OBJCE_P(hitsuji->bootstrap), hitsuji->bootstrap, ZEND_STRL("dirs"), 1 TSRMLS_CC);

    if (zend_hash_find(Z_ARRVAL_P(zdirs), key, strlen(key) + 1, (void **)&zdir) == FAILURE
        || IS_STRING != Z_TYPE_PP(zdir)) {
        return NULL;
    }
    return Z_STRVAL_PP(zdir);
}

/**
 * hitSuji\Bootstrapクラス変数:patterns
 *
 * @param  const char *key
 * @return char
 */
char *getPattern(const char *key)
{
    hitsuji_t *hitsuji;
    zval *zdirs, **zpat;

    hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    zdirs = zend_read_property(Z_OBJCE_P(hitsuji->bootstrap), hitsuji->bootstrap, ZEND_STRL("patterns"), 1 TSRMLS_CC);

    if (zend_hash_find(Z_ARRVAL_P(zdirs), key, strlen(key) + 1, (void **)&zpat) == FAILURE
        || IS_STRING != Z_TYPE_PP(zpat)) {
        return NULL;
    }

    return Z_STRVAL_PP(zpat);
}

/**
 * hitSuji\Bootstrapクラス関数:makeNonce
 *
 * @param  string $seed
 * @return string
 */
zval *makeNonce(zval *retval_ptr, zval *seed)
{
    hitsuji_t *hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);

    return zend_call_method_with_1_params(
        (zval **)&hitsuji->bootstrap, 
        Z_OBJCE_P(hitsuji->bootstrap), 
        NULL, 
        "nonce", 
        (zval **)&retval_ptr, 
        seed
    );
}

/**
 * 入力値の取得
 *
 * @access public
 * @param zval   retval_p     取得した値
 * @param string key          添字
 * @param string tack         参照先テーブル
 * @param zval   *request_ptr リクエスト配列
 * @return int
 */
int getRequestValue(zval *retval_p, const char *key, const char *track, zval *request_ptr)
{
    zval **value;
    zval *array_ptr = NULL;

    if (strncasecmp(track, "post", 4) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_POST];
    } else if (strncasecmp(track, "get", 3) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_GET];
    } else if (strncasecmp(track, "cookie", 6) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_COOKIE];
    } else if (strncasecmp(track, "route", 5) == 0) {
        hitsuji_t *hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);
        array_ptr = zend_read_property(Z_OBJCE_P(hitsuji->router), hitsuji->router, ZEND_STRL("vars"), 1 TSRMLS_CC);
        if (!zend_is_true(array_ptr) || IS_ARRAY != Z_TYPE_P(array_ptr)) {
            return 0;
        }
    } else if (strncasecmp(track, "server", 6) == 0) {
        array_ptr = PG(http_globals)[TRACK_VARS_SERVER];
    } else {
        if (zend_hash_num_elements(Z_ARRVAL_P(request_ptr)) == 0) {
            hitsuji_t *hitsuji = (hitsuji_t *) zend_object_store_get_object(hitsuji_object_ptr TSRMLS_CC);
            zval *zvars;

            zend_hash_merge(
                Z_ARRVAL_P(request_ptr), 
                Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_POST]), 
                (copy_ctor_func_t)zval_add_ref, 
                NULL, 
                sizeof(zval *), 
                1
            );

            zend_hash_merge(
                Z_ARRVAL_P(request_ptr), 
                Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_GET]), 
                (copy_ctor_func_t)zval_add_ref, 
                NULL, 
                sizeof(zval *), 
                1
            );

            zend_hash_merge(
                Z_ARRVAL_P(request_ptr), 
                Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_COOKIE]), 
                (copy_ctor_func_t)zval_add_ref, 
                NULL, 
                sizeof(zval *), 
                1
            );

            zvars = zend_read_property(Z_OBJCE_P(hitsuji->router), hitsuji->router, ZEND_STRL("vars"), 1 TSRMLS_CC);
            if (zend_is_true(zvars) && IS_ARRAY == Z_TYPE_P(zvars)) {
                zend_hash_merge(
                    Z_ARRVAL_P(request_ptr), 
                    Z_ARRVAL_P(zvars), 
                    (copy_ctor_func_t)zval_add_ref, 
                    NULL, 
                    sizeof(zval *), 
                    1
                );
            }
        }
        array_ptr = request_ptr;
    }

    if (!zend_is_true(array_ptr) || IS_ARRAY != Z_TYPE_P(array_ptr)) {
        return 0;
    }

    if (SUCCESS != zend_hash_find(Z_ARRVAL_P(array_ptr), key, strlen(key) + 1, (void **) &value)) { 
        return 0;
    }

    ZVAL_ZVAL(retval_p, *value, 1, 0);
    return 1;
}

#endif      // #ifndef HAVE_HITSUJI_UTILITY

