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

#ifndef HAVE_HITSUJI_CLASS_ROUTER_H
#define HAVE_HITSUJI_CLASS_ROUTER_H

/**
 * ルーティング登録
 */
typedef struct {
    zval method;
    zval uri;
    zval filename;
    zval *callable;
    int is_callable;
} hitsuji_route_t;

/**
 * \hitSuji\Routerクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    HashTable *routes;
    zval dir;
    zval filename;
    zval *callable;
    int is_callable;
} hitsuji_router_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(HSJRouter, __construct);
PHP_METHOD(HSJRouter, __destruct);
PHP_METHOD(HSJRouter, run);
PHP_METHOD(HSJRouter, always);
PHP_METHOD(HSJRouter, on);


ZEND_BEGIN_ARG_INFO_EX(HSJRouter_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJRouter_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, param1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJRouter_2_param, 0, ZEND_RETURN_VALUE, 2)
    ZEND_ARG_INFO(0, param1)
    ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJRouter_3_param, 0, ZEND_RETURN_VALUE, 3)
    ZEND_ARG_INFO(0, param1)
    ZEND_ARG_INFO(0, param2)
    ZEND_ARG_INFO(0, param3)
ZEND_END_ARG_INFO()

extern zend_function_entry hitsuji_router_class_methods[];

#else
#   ifndef HAVE_HITSUJI_CLASS_ROUTER
#   define HAVE_HITSUJI_CLASS_ROUTER

/**
 * クラスの実装部分
 */
zend_function_entry hitsuji_router_class_methods[] = {
    PHP_ME(HSJRouter, __construct, HSJRouter_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HSJRouter, __destruct,  HSJRouter_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HSJRouter, run,         HSJRouter_2_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJRouter, always,      HSJRouter_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJRouter, on,          HSJRouter_3_param, ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * 配列の削除規定
 *
 * @return object
 */
static void router_destroy_route(hitsuji_route_t *route)
{
    zval_dtor(&route->method);
    zval_dtor(&route->uri);
    zval_dtor(&route->filename);
    zval_ptr_dtor(&route->callable);
}

/**
 * HSJRouterクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJRouter, __construct)
{
    hitsuji_router_t *self;
    zval zkey, *zvars, *zdir;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_router_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* テンプレートファイル保存ディレクトリの取得と設定 */
    ZVAL_STRING(&self->dir, HITSUJI_G(page_path), 1);

    /* hashテーブルの初期化 */
    ALLOC_HASHTABLE(self->routes);
    zend_hash_init(self->routes, 0, NULL, (dtor_func_t) router_destroy_route, 0);

    /* URL中の変数保存の初期化 */
    if (NULL == HITSUJI_G(routes)) {
        ALLOC_INIT_ZVAL(HITSUJI_G(routes));
        array_init(HITSUJI_G(routes));
    }
    array_all_clean(HITSUJI_G(routes));

    /* 取得URL変数の初期化 */
    if (NULL == HITSUJI_G(page)) {
        ALLOC_INIT_ZVAL(HITSUJI_G(page));
    }
}

/**
 * HSJRouterクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJRouter, __destruct)
{
    hitsuji_router_t *self;
    zval *zvars;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_router_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (NULL != HITSUJI_G(page)) {
        zval_ptr_dtor(&HITSUJI_G(page));
        HITSUJI_G(page) = NULL;
    }

    if (NULL != HITSUJI_G(routes)) {
        array_all_clean(HITSUJI_G(routes));
        zval_ptr_dtor(&HITSUJI_G(routes));
        HITSUJI_G(routes) = NULL;
    }

    zval_dtor(&self->dir);

    if (NULL != self->callable) {
        zval_ptr_dtor(&self->callable);
    }

    if (NULL != self->routes) {
#if PHP_VERSION_ID > 50520
        zend_hash_destroy(self->routes);
#endif
        FREE_HASHTABLE(self->routes);
    }

    if (zend_is_true(&self->filename)) {
        zval_dtor(&self->filename);
    }
}

/**
 * 登録アクションの実行
 *
 * Example:
 * <code>
 * $router->run();
 * $router->run('get', '/user/:id');
 * </code>
 *
 * @access public
 * @return boolean
 */
PHP_METHOD(HSJRouter, run)
{
    hitsuji_router_t *self;
    hitsuji_route_t *route = NULL;
    HashPosition position;
    zval zurl;
    const char *method = NULL;
    char *uri = NULL;
    uint method_len = 0, uri_len = 0;

    /* リクエストの取得 */
    if (SG(request_info).request_method) {
        zval **url_pp;
        int pos;

        if (SUCCESS == zend_hash_find(Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_SERVER]), "REQUEST_URI", 12, (void **) &url_pp)) { 
            ZVAL_STRINGL(&zurl, Z_STRVAL_PP(url_pp), Z_STRLEN_PP(url_pp), 1);
        } else {
            ZVAL_STRING(&zurl, SG(request_info).path_translated, 1);
        }

        method     = SG(request_info).request_method;
        method_len = strlen(method);
        uri        = Z_STRVAL_PP(url_pp);
        uri_len    = Z_STRLEN_PP(url_pp);

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
    } // if (SG(request_info).request_method)

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ss", &method, &method_len, &uri, &uri_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_router_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (NULL != self->routes && zend_hash_num_elements(self->routes) != 0) {
        /* ルーティングルールをループして検索 */
        for (zend_hash_internal_pointer_reset_ex(self->routes, &position);
             zend_hash_get_current_data_ex(self->routes, (void**) &route, &position) == SUCCESS;
             zend_hash_move_forward_ex(self->routes, &position)) {

            /* メソッドの比較 */
            if ((method_len == Z_STRLEN(route->method)) && 
                (strncasecmp(method, Z_STRVAL(route->method), method_len) == SUCCESS)) {

                zval replace_val, zmatch = {0};
                pcre_cache_entry *pcre = NULL;
                char *assoc_pattern = NULL;
                int pattern_len = 0, replace_count=0;

                /* ルーティングルールを正規表現にコンパイル */
                ZVAL_STRINGL(&replace_val, "(?P<\\1>[^/]+)", strlen("(?P<\\1>[^/]+)"), 0);
                assoc_pattern = php_pcre_replace(
                    "/:([^\\/]+)/", strlen("/:([^\\/]+)/"),
                    Z_STRVAL(route->uri), Z_STRLEN(route->uri),
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
                        (char *)uri, strlen(uri), 
                        &zmatch, HITSUJI_G(routes), 
                        0, 0, 0, 0 TSRMLS_CC
                    );
                    array_trim_index(HITSUJI_G(routes));

                    if (zend_is_true(&zmatch)) {
                        ZVAL_STRING(HITSUJI_G(page), Z_STRVAL(route->uri), 1);
                        if (route->is_callable) {
                            hitsuji_call_function_args(route->callable, NULL, HITSUJI_G(routes));
                        } else {
                            /* extract() PHP関数の実行 */
                            hitsuji_execute_scripts(Z_STRVAL(route->filename));
                        }

                        RETURN_TRUE;
                    }
                } // if (NULL != pcre)
            }
        } // for
    } // if

    if (self->is_callable) {
        hitsuji_call_function_0_params(self->callable, NULL);
    } else {
        hitsuji_execute_scripts(Z_STRVAL(self->filename));
    }

    RETURN_TRUE;
}

/**
 * デフォルトアクションの登録
 *
 * Example:
 * <code>
 * $router->always(function() {
 *     echo '<html><head><title></title></head><body></body></html>';
 * });
 * </code>
 *
 * @access public
 * @param mixed callback
 * @return mixed
 */
PHP_METHOD(HSJRouter, always)
{
    hitsuji_router_t *self;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_router_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        self->callable = callable;
        Z_ADDREF_P(self->callable);
        self->is_callable = 1;
    } else if (IS_STRING == Z_TYPE_P(callable)) {
        char filename[256];

        /* ディレクトリパスの追加 */
        strcpy(filename, Z_STRVAL(self->dir));
        /* contentファイル名の追加 */
        strcat(filename, Z_STRVAL_P(callable));

        ZVAL_STRING(&self->filename, filename, 1);
        self->is_callable = 0;
    } else {
        zend_throw_exception(hitsuji_exception_ce, "An argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    RETURN_CHAIN();
}

/**
 * ルーティング＆メソッドアクションの登録
 *
 * Example:
 * <code>
 * $router->on('get', '/home/:id', function($id) {
 *     echo '<html><head><title></title></head><body></body></html>';
 * });
 * </code>
 *
 * @access public
 * @param string method
 * @param string routing
 * @param mixed callback
 * @return mixed
 */
PHP_METHOD(HSJRouter, on)
{
    hitsuji_router_t *self;
    hitsuji_route_t route;
    char *method = NULL, *uri = NULL;
    unsigned int method_len, uri_len;
    zval *callable;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ssz", &method, &method_len, &uri, &uri_len, &callable) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_router_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* コールバックが指定されているかチェック */
    if (zend_is_callable(callable, 0, NULL TSRMLS_CC)) {
        route.callable = callable;
        Z_ADDREF_P(route.callable);

        route.is_callable = 1;
    } else if (IS_STRING == Z_TYPE_P(callable)) {
        char filename[256];

        /* ディレクトリパスの追加 */
        strcpy(filename, Z_STRVAL(self->dir));
        /* contentファイル名の追加 */
        strcat(filename, Z_STRVAL_P(callable));

        ZVAL_STRING(&route.filename, filename, 1);
        route.is_callable = 0;
    } else {
        zend_throw_exception(hitsuji_exception_ce, "Third argument is not callable.", 0 TSRMLS_CC);
        RETURN_FALSE;
    }

    ZVAL_STRINGL(&route.method, method, method_len, 1);
    ZVAL_STRINGL(&route.uri, uri, uri_len, 1);

    /* ハッシュテーブルへ登録 */
    zend_hash_next_index_insert(
        self->routes, 
        (void**) &route, 
        sizeof(hitsuji_route_t), 
        NULL
    );
    RETURN_CHAIN();
}

#   endif       /* #ifndef HAVE_HITSUJI_CLASS_ROUTER */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_ROUTER_H */
