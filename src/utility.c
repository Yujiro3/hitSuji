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
#include "ext/standard/md5.h"
#include "zend_alloc.h"
#include "zend_interfaces.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_UTILITY_H
#   include "src/utility.h"
#endif

/**
 * 入力値の取得
 *
 * @access public
 * @param zval   retval_ptr   取得した値
 * @param string key          添字
 * @param string tack         参照先テーブル
 * @return int
 */
int get_request_value(zval *retval_ptr, const char *key, const char *track)
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
        if (NULL == HITSUJI_G(routes)) {
            return 0;
        }
        array_ptr = HITSUJI_G(routes);
        if (!zend_is_true(array_ptr) || IS_ARRAY != Z_TYPE_P(array_ptr)) {
            return 0;
        }
    } else if (strncasecmp(track, "server", 6) == 0) {
        if (PG(auto_globals_jit)) {
            zend_is_auto_global("_SERVER", sizeof("_SERVER") - 1 TSRMLS_CC);
        }
        array_ptr = PG(http_globals)[TRACK_VARS_SERVER];
    } else {
        if (zend_hash_num_elements(Z_ARRVAL_P(HITSUJI_G(requests))) == 0) {
            zval *zvars;

            zend_hash_merge(
                Z_ARRVAL_P(HITSUJI_G(requests)), 
                Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_POST]), 
                (copy_ctor_func_t)zval_add_ref, 
                NULL, 
                sizeof(zval *), 
                1
            );

            zend_hash_merge(
                Z_ARRVAL_P(HITSUJI_G(requests)), 
                Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_GET]), 
                (copy_ctor_func_t)zval_add_ref, 
                NULL, 
                sizeof(zval *), 
                1
            );

            zend_hash_merge(
                Z_ARRVAL_P(HITSUJI_G(requests)), 
                Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_COOKIE]), 
                (copy_ctor_func_t)zval_add_ref, 
                NULL, 
                sizeof(zval *), 
                1
            );

            if (NULL != HITSUJI_G(routes) && zend_is_true(HITSUJI_G(routes)) && IS_ARRAY == Z_TYPE_P(HITSUJI_G(routes))) {
                zend_hash_merge(
                    Z_ARRVAL_P(HITSUJI_G(requests)), 
                    Z_ARRVAL_P(HITSUJI_G(routes)), 
                    (copy_ctor_func_t)zval_add_ref, 
                    NULL, 
                    sizeof(zval *), 
                    1
                );
            }
        }
        array_ptr = HITSUJI_G(requests);
    }

    if (!array_ptr || !zend_is_true(array_ptr) || IS_ARRAY != Z_TYPE_P(array_ptr)) {
        return 0;
    }

    if (SUCCESS != zend_hash_find(Z_ARRVAL_P(array_ptr), key, strlen(key) + 1, (void **) &value)) { 
        return 0;
    }

    ZVAL_ZVAL(retval_ptr, *value, 1, 0);
    return 1;
}

/**
 * NONCE値の取得
 *
 * @access public
 * @param char *nonce      nonce値
 * @param const char *seed 種
 * @return char
 */
char *get_nonce(const char *seed)
{
    char *nonce = NULL;
    PHP_MD5_CTX context;
    time_t now; 
    long timer;
    char plain[256];
    char trimed[213];
    char gseed[31];
    char md5str[33];
    unsigned char digest[16];

    now = time(NULL);
    timer = (long)(now / HITSUJI_G(lifetime));

    /* Global SEEDのトリミング */
    if (strlen(HITSUJI_G(seed)) >= 30) {
        memcpy(gseed, HITSUJI_G(seed), 29);
        gseed[30] = '\0';
    } else {
        strcpy(gseed, HITSUJI_G(seed));
    }

    /* SEEDのトリミング */
    if (strlen(trimed) >= 212) {
        memcpy(trimed, seed, 211);
        trimed[212] = '\0';
    } else {
        strcpy(trimed, seed);
    }

    sprintf(plain, "%d:%s:%s", timer, gseed, trimed);
    plain[256] = '\0';
    md5str[0] = '\0';
    PHP_MD5Init(&context);
    PHP_MD5Update(&context, plain, strlen(plain));
    PHP_MD5Final(digest, &context);
    make_digest_ex(md5str, digest, 16);

    /* substr($hashed, -14, 10); */
    md5str[28] = '\0';

    nonce = (char *)emalloc(11);
    strcpy(nonce, &md5str[18]);

    return nonce;
}

/**
 * ファイル名の取得
 *
 * @access public
 * @param char *nonce      nonce値
 * @param const char *seed 種
 * @return char
 */
char *get_filename(char *dir, char *name)
{
    char *filename;

    filename = (char *)emalloc((strlen(dir) + strlen(name) + 1));

    /* ディレクトリパスの追加 */
    strcpy(filename, dir);

    /* contentファイル名の追加 */
    strcat(filename, name);

    return filename;
}

#endif      // #ifndef HAVE_HITSUJI_UTILITY

