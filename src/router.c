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

#ifndef HAVE_HITSUJI_ROUTER
#define HAVE_HITSUJI_ROUTER

#include "php.h"
#include "php_ini.h"
#include "SAPI.h"
#include "ext/pcre/php_pcre.h"
#include "zend_alloc.h"
#include "zend_interfaces.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_ROUTER_H
#   include "src/router.h"
#endif

static void php_head_apply_header_list_to_hash(void *data, void *arg TSRMLS_DC)
{
        sapi_header_struct *sapi_header = (sapi_header_struct *)data;

        if (arg && sapi_header) {
                add_next_index_string((zval *)arg, (char *)(sapi_header->header), 1);
        }
}


/**
 * URLの取得
 *
 * @access public
 * @return *char
 */
char *router_get_url(void)
{
    char *result = NULL;
    char *uri = NULL;
    uint uri_len = 0;

    /* リクエストの取得 */
    if (SG(request_info).request_method) {
        zval **url_pp;
        int pos;

        if (PG(auto_globals_jit)) {
            zend_is_auto_global("_SERVER", sizeof("_SERVER") - 1 TSRMLS_CC);
        }

        if (PG(http_globals)[TRACK_VARS_SERVER] &&
            zend_hash_find(Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_SERVER]), "REQUEST_URI", sizeof("REQUEST_URI"), (void **) &url_pp) == SUCCESS
        ) {
            uri = Z_STRVAL_PP(url_pp);
        } else {
            uri = SG(request_info).request_uri;
        }
        uri_len = strlen(uri);

        /* ?より後を切り捨て */
        for (pos=0; pos <= uri_len; pos++) {
            if (uri[pos] == '?') {
                uri[pos] = '\0';
                uri_len = pos;
                break;
            }
        }

        /* 最後の/を破棄 */
        if (uri[uri_len -1] == '/') {
            uri_len --;
            uri[uri_len] = '\0';
        }
    } else {
        return NULL;
    } // if (SG(request_info).request_method)

    if (0 >= uri_len) {
        return NULL;
    }

    result = (char *)emalloc(uri_len);
    strcpy(result, uri);

    return result;
}

/**
 * methodの取得
 *
 * @access public
 * @return *char
 */
char *router_get_method(void)
{
    char *result = NULL;
    const char *method = NULL;

    /* リクエストの取得 */
    if (SG(request_info).request_method) {
        method     = SG(request_info).request_method;

        result = (char *)emalloc(strlen(method));
        strcpy(result, method);
    } else {
        return NULL;
    } // if (SG(request_info).request_method)

    return result;
}

/**
 * methodがマッチしているかチェック
 *
 * @access public
 * @param  char *method
 * @param  zval *array
 * @return int
 */
int router_is_method(char *method, zval *array)
{
    HashPosition pos;
    zval **row;

    if (IS_ARRAY != Z_TYPE_P(array)) {
        return 0;
    }

    /* 判定条件の取得 */
    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
         zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&row, &pos) == SUCCESS;
         zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {
        if (IS_STRING == Z_TYPE_PP(row)) {
            if (strncasecmp(Z_STRVAL_PP(row), method, strlen(method)) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * URLがマッチしているかチェック
 *
 * @access public
 * @param  char *url
 * @param  zval *array
 * @return int
 */
int router_is_route(char *url, char *route)
{
    zval replace_val, zmatch = {0};
    pcre_cache_entry *pcre = NULL;
    char *assoc_pattern = NULL;
    int pattern_len = 0, replace_count=0;

    /* ルーティングルールを正規表現にコンパイル */
    ZVAL_STRINGL(&replace_val, "(?P<\\1>[^/]+)", strlen("(?P<\\1>[^/]+)"), 0);
    assoc_pattern = php_pcre_replace(
        "/:([^\\/]+)/", strlen("/:([^\\/]+)/"),
        route, strlen(route),
        &replace_val,
        0, &pattern_len, -1, 
        &replace_count TSRMLS_CC
    );

    if (pattern_len) {
        /* URLマッチ用パターンの作成 */
        char pattern[pattern_len + 3];
        sprintf(pattern,"~^%s~", assoc_pattern);
        pcre = pcre_get_compiled_regex_cache(pattern, pattern_len + 3 TSRMLS_CC);
        efree(assoc_pattern);
    }

    if (NULL != pcre) {
        php_pcre_match_impl(
            pcre,
            (char *)url, strlen(url), 
            &zmatch, HITSUJI_G(routes), 
            0, 0, 0, 0 TSRMLS_CC
        );
        array_trim_index(HITSUJI_G(routes));

        if (zend_is_true(&zmatch)) {

            return 1;
        }
    } // if (NULL != pcre)

    return 0;
}

/**
 * ファイル名の取得
 *
 * @access public
 * @param const char *name ファイル名
 * @return char
 */
char *router_get_filename(char *name)
{
    char *filename;
    char *dir;

    dir = (char *)HITSUJI_G(page_path);
    filename = (char *)emalloc((strlen(dir) + strlen(name) + 1));

    /* ディレクトリパスの追加 */
    strcpy(filename, dir);

    /* contentファイル名の追加 */
    strcat(filename, name);

    return filename;
}

/**
 * URLがマッチしているかチェック
 *
 * @access public
 * @param  char *url
 * @param  zval *array
 * @return int
 */
int router_fire_action(zval *array)
{
    HashPosition pos;
    zval **row;

    if (zend_is_callable(array, 0, NULL TSRMLS_CC)) {
        hitsuji_call_function_args(array, NULL, HITSUJI_G(routes));
        return 1;
    }

    if (IS_STRING == Z_TYPE_P(array)) {
        char *filename = (char *)router_get_filename(Z_STRVAL_P(array));
        hitsuji_execute_scripts(filename);
        efree(filename);
        return 1;
    }

    if (IS_ARRAY != Z_TYPE_P(array)) {
        return 0;
    }

    /* 判定条件の取得 */
    for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
         zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&row, &pos) == SUCCESS;
         zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
    ) {
        if (zend_is_callable(*row, 0, NULL TSRMLS_CC)) {
            hitsuji_call_function_args(array, NULL, HITSUJI_G(routes));
            break;
        }

        if (IS_STRING == Z_TYPE_PP(row)) {
            if (strncasecmp(Z_STRVAL_PP(row), "get", 3) == 0) {
            } else if (strncasecmp(Z_STRVAL_PP(row), "post", 3) == 0) {
            } else {
                char *filename = (char *)router_get_filename(Z_STRVAL_PP(row));
                hitsuji_execute_scripts(filename);
                efree(filename);
                break;
            }
        }
    }
    return 1;
}

#endif      // #ifndef HAVE_HITSUJI_ROUTER

