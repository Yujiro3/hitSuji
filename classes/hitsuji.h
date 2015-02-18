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
} hitsuji_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(hitSuji, nonce);
PHP_METHOD(hitSuji, template);
PHP_METHOD(hitSuji, router);
PHP_METHOD(hitSuji, request);
PHP_METHOD(hitSuji, delegate);
PHP_METHOD(hitSuji, quick);


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
    PHP_ME(hitSuji, nonce,       hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, template,    hitSuji_0_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, router,      hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, request,     hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, delegate,    hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(hitSuji, quick,       hitSuji_1_param,  ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * hitSujiクラス関数:nonce
 *
 * @param  string $seed
 * @return string
 */
PHP_METHOD(hitSuji, nonce)
{
    char *nonce;
    char *seed = NULL;
    uint seed_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &seed, &seed_len) == FAILURE) {
        RETURN_FALSE;
    }
    nonce = getNonce(seed);

    RETVAL_STRING(nonce, 1);
    efree(nonce);
}

/**
 * hitSujiクラス関数:template
 *
 * @return object
 */
PHP_METHOD(hitSuji, template)
{
    zval *view;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    /* ビューの初期化 */
    ALLOC_INIT_ZVAL(view);
    object_init_ex(view, hitsuji_view_ce);
    CALL_METHOD(HSJView, __construct, NULL, view);

    /* 返り値へオブジェクトを渡す */
    RETURN_ZVAL(view, 1, 1);
}

/**
 * hitSujiクラス関数:router
 *
 * Example:
 * <code>
 * hitSuji::router([
 *     ''        => 'simple.php',
 *     'usr/:id' => [
 *         function ($id) {
 *             echo "usr:{$id}\n";
 *         }, 'get'
 *     ]
 * ]);
 * </code>
 * @return object
 */
PHP_METHOD(hitSuji, router)
{
    zval *routes = NULL;
    zval *always = NULL;
    char *url = NULL, *method = NULL;
    int  matched = 0;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &routes) == FAILURE) {
        RETURN_FALSE;
    }
    url    = routerGetURL();
    method = routerGetMethod();

    if (HITSUJI_G(routes) == NULL) {
        ALLOC_INIT_ZVAL(HITSUJI_G(routes));
        array_init(HITSUJI_G(routes));
    }

    if (NULL == HITSUJI_G(page)) {
        ALLOC_INIT_ZVAL(HITSUJI_G(page));
    }

    /* プロパティを変数へ */
    {
        HashPosition pos;
        zval **row;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(routes), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(routes), (void **)&row, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(routes), &pos)
        ) {
            char *route = NULL;
            uint route_len = 0;
            ulong index;

            zend_hash_get_current_key_ex(Z_ARRVAL_P(routes), &route, &route_len, &index, 0, &pos);

            if (NULL != url && strlen(route)) {
                /* methodとルートのマッチ */
                if (routerIsMethod(method, *row) && routerIsRoute(url, route)) {
                    ZVAL_STRING(HITSUJI_G(page), route, 1);
                    if (routerFireAction(*row)) {
                        matched = 1;
                        break;
                    }
                }
            } else if (0 == strlen(route)) {
                always = *row;
            }
        } // for
    }

    if (!matched) {
        if (zend_is_callable(always, 0, NULL TSRMLS_CC)) {
            hitsuji_call_function_0_params(always, NULL);
        } else if (IS_STRING == Z_TYPE_P(always)) {
            char *filename = getFilename(HITSUJI_G(page_path), Z_STRVAL_P(always));
            hitsuji_execute_scripts(filename);
            efree(filename);
        }
    }

    /* メモリの開放 */
    if (NULL != HITSUJI_G(page)) {
        zval_ptr_dtor(&HITSUJI_G(page));
        HITSUJI_G(page) = NULL;
    }

    if (NULL != HITSUJI_G(routes)) {
        zval_ptr_dtor(&HITSUJI_G(routes));
        HITSUJI_G(routes) = NULL;
    }

    if (NULL != method) {
        efree(method);
    }
    if (NULL != url) {
        efree(url);
    }
}

/**
 * hitSujiクラス関数:request
 *
 * Example:
 * <code>
 * hitSuji::request([
 *     'id'   => number,
 *     'name' => ['string', 'require'],
 *     'email'=> ['email', 'request'],
 * ]);
 * </code>
 *
 * @return object
 */
PHP_METHOD(hitSuji, request)
{
    zval *options = NULL;
    int  valid = 0;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &options) == FAILURE) {
        RETURN_FALSE;
    }

    if (HITSUJI_G(vars) == NULL) {
        ALLOC_INIT_ZVAL(HITSUJI_G(vars));
        array_init(HITSUJI_G(vars));
    }

    if (NULL == HITSUJI_G(checks)) {
        ALLOC_INIT_ZVAL(HITSUJI_G(checks));
        array_init(HITSUJI_G(checks));
    }

    valid = hitsuji_verifies(options);

    RETVAL_ZVAL(HITSUJI_G(vars), 1, 1);

    array_all_clean(HITSUJI_G(vars));
    array_all_clean(HITSUJI_G(checks));

    if (NULL != HITSUJI_G(checks)) {
        zval_ptr_dtor(&HITSUJI_G(checks));
        HITSUJI_G(checks) = NULL;
    }
}

/**
 * hitSujiクラス関数:delegate
 *
 * Example:
 * <code>
 * hitSuji::delegate([
 *     'data' => [
 *         'id' => $_GET['id'],
 *         'sid'=> $_GET['sid']
 *     ],
 *     'bind' => [
 *         'id'   => 'number',
 *         'name' => array('string', 'require'),
 *         'email'=> array('email', 'request'),
 *     ],
 *     'parse'=> function ($data) {
 *         return $data;
 *     },
 *     'action'=> function ($data) {
 *         return [true, $data];
 *     },
 *     'always' => function ($data) {
 *         echo 'always!';
 *     },
 *     'done' => function ($data) {
 *         echo 'done!';
 *     },
 *     'fail' => function ($data) {
 *         echo 'fail!';
 *     }
 * ]);
 * </code>
 *
 * @return object
 */
PHP_METHOD(hitSuji, delegate)
{
    zval *property = NULL;
    zval *data = NULL;
    zval *parse = NULL, *action = NULL;
    zval *always = NULL, *done = NULL, *fail = NULL;
    zval *retval = NULL;
    int  valid = 0, nvalid = 0;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &property) == FAILURE) {
        RETURN_FALSE;
    }

    /* プロパティを変数へ */
    {
        HashPosition pos;
        zval **row;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(property), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(property), (void **)&row, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(property), &pos)
        ) {
            char *key = NULL;
            uint key_len = 0;
            ulong index;

            zend_hash_get_current_key_ex(Z_ARRVAL_P(property), &key, &key_len, &index, 0, &pos);
            if (strncasecmp(key, "data", 4) == 0) {
                if (zend_is_true(*row)) {
                    data = *row;
                }
            } else if (strncasecmp(key, "bind", 4) == 0) {
                if (zend_is_true(*row) && IS_ARRAY == Z_TYPE_PP(row)) {
                    if (NULL == HITSUJI_G(vars)) {
                        ALLOC_INIT_ZVAL(HITSUJI_G(vars));
                        array_init(HITSUJI_G(vars));
                    }

                    if (NULL == HITSUJI_G(checks)) {
                        ALLOC_INIT_ZVAL(HITSUJI_G(checks));
                        array_init(HITSUJI_G(checks));
                    }

                    valid = hitsuji_verifies(*row);
                }
            } else {
                if (zend_is_callable(*row, 0, NULL TSRMLS_CC)) {
                    if (strncasecmp(key, "parse", 5) == 0) {
                        parse = *row;
                    } else if (strncasecmp(key, "action", 6) == 0) {
                        action = *row;
                    } else if (strncasecmp(key, "always", 6) == 0) {
                        always = *row;
                    } else if (strncasecmp(key, "done", 4) == 0) {
                        done = *row;
                    } else if (strncasecmp(key, "fail", 4) == 0) {
                        fail = *row;
                    }
                } // if (zend_is_callable( *row, 0, NULL TSRMLS_CC)) 
            } // if (strncasecmp(key, "vars", 4)) 
        }
    }

    /* データの準備 */
    if (NULL == data && NULL == HITSUJI_G(vars)) {
        ALLOC_INIT_ZVAL(data);
        array_init(data);
    } else if (NULL != data && NULL != HITSUJI_G(vars)) {
        zend_hash_merge(
            Z_ARRVAL_P(data), 
            Z_ARRVAL_P(HITSUJI_G(vars)), 
            (copy_ctor_func_t)zval_add_ref, 
            NULL, 
            sizeof(zval *), 
            1
        );
    } else {
        data = HITSUJI_G(vars);
    }

    /* データの解析処理 */
    if (NULL != parse && zend_is_callable(parse, 0, NULL TSRMLS_CC)) {
        hitsuji_call_function_1_params(parse, (zval **)&retval, data);
        zval_ptr_dtor(&data);
        data = retval;
    }

    /* Nonce値の判定 */
    if (NULL != HITSUJI_G(page)) {
        nvalid = hitsuji_nonce_verify(Z_ARRVAL_P(HITSUJI_G(page)));
    }

    if (valid && nvalid) {
        int result;

        /* アクション処理の実行 */
        if (NULL != action && zend_is_callable(action, 0, NULL TSRMLS_CC)) {
            hitsuji_call_function_1_params(action, (zval **)&retval, data);
            zval_ptr_dtor(&data);
            data = retval;
        }

        data = array_bool_data(&result, data);
        if (result) {
            /* 成功時処理の実行 */
            if (NULL != done && zend_is_callable(done, 0, NULL TSRMLS_CC)) {
                hitsuji_call_function_1_params(done, (zval **)&retval, data);
                zval_ptr_dtor(&data);
                data = retval;
            }
        } else {
            /* 失敗時処理の実行 */
            if (NULL != fail && zend_is_callable(fail, 0, NULL TSRMLS_CC)) {
                hitsuji_call_function_1_params(fail, (zval **)&retval, data);
                zval_ptr_dtor(&data);
                data = retval;
            }
        }
    } else {
        if (nvalid) {
            /* 失敗時処理の実行 */
            if (NULL != fail && zend_is_callable(fail, 0, NULL TSRMLS_CC)) {
                hitsuji_call_function_1_params(fail, (zval **)&retval, data);
                zval_ptr_dtor(&data);
                data = retval;
            }
        } else {
            /* デフォルト処理の実行 */
            if (NULL != always && zend_is_callable(always, 0, NULL TSRMLS_CC)) {
                hitsuji_call_function_1_params(always, (zval **)&retval, data);
                zval_ptr_dtor(&data);
                data = retval;
            }
        }
    }
    RETVAL_ZVAL(data, 1, 1);

    /* グローバル変数のクリア */
    array_all_clean(HITSUJI_G(vars));
    array_all_clean(HITSUJI_G(checks));

    if (NULL != HITSUJI_G(checks)) {
        zval_ptr_dtor(&HITSUJI_G(checks));
        HITSUJI_G(checks) = NULL;
    }

    if (NULL != HITSUJI_G(vars)) {
        zval_ptr_dtor(&HITSUJI_G(vars));
        HITSUJI_G(vars) = NULL;
    }
}

/**
 * hitSujiクラス関数:quick
 *
 * Example:
 * <code>
 * hitSuji::action([
 *     'data' => [
 *         'id' => $_GET['id'],
 *         'sid'=> $_GET['sid']
 *     ],
 *     'parse'=> function ($data) {
 *         return $data;
 *     },
 *     'action'=> function ($data) {
 *         return [true, $data];
 *     },
 *     'done' => function ($data) {
 *         echo 'done!';
 *     },
 *     'fail' => function ($data) {
 *         echo 'fail!';
 *     }
 * ]);
 * </code>
 *
 * @param  array  $property
 * @return string
 */
PHP_METHOD(hitSuji, quick)
{
    zval *property = NULL, *data = NULL, *parse = NULL;
    zval *action = NULL, *done = NULL, *fail = NULL;
    zval *retval = NULL;
    int  result;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &property) == FAILURE) {
        RETURN_FALSE;
    }

    /* プロパティを変数へ */
    {
        HashPosition pos;
        zval **row;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(property), &pos);
             zend_hash_get_current_data_ex(Z_ARRVAL_P(property), (void **)&row, &pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL_P(property), &pos)
        ) {
            char *key = NULL;
            uint key_len = 0;
            ulong index;

            zend_hash_get_current_key_ex(Z_ARRVAL_P(property), &key, &key_len, &index, 0, &pos);

            if (strncasecmp(key, "data", 4) == 0) {
                if (zend_is_true(*row)) {
                    data = *row;
                }
            } else {
                if (zend_is_callable(*row, 0, NULL TSRMLS_CC)) {
                    if (strncasecmp(key, "parse", 5) == 0) {
                        parse = *row;
                    } else if (strncasecmp(key, "action", 6) == 0) {
                        action = *row;
                    } else if (strncasecmp(key, "done", 4) == 0) {
                        done = *row;
                    } else if (strncasecmp(key, "fail", 4) == 0) {
                        fail = *row;
                    }
                } // if (zend_is_callable(*row, 0, NULL TSRMLS_CC)) 
            } // if (strncasecmp(key, "data", 4)) 
        }
    }

    /* データの準備 */
    if (NULL == data) {
        ALLOC_INIT_ZVAL(data);
        array_init(data);
    }

    /* データの解析処理 */
    if (NULL != parse && zend_is_callable(parse, 0, NULL TSRMLS_CC)) {
        hitsuji_call_function_1_params(parse, (zval **)&retval, data);
        zval_ptr_dtor(&data);
        data = retval;
    }

    /* アクション処理の実行 */
    if (NULL != action && zend_is_callable(action, 0, NULL TSRMLS_CC)) {
        hitsuji_call_function_1_params(action, (zval **)&retval, data);
        zval_ptr_dtor(&data);
        data = retval;
    }

    data = array_bool_data(&result, data);
    if (result) {
        /* 失敗時処理の実行 */
        if (NULL != done && zend_is_callable(done, 0, NULL TSRMLS_CC)) {
            hitsuji_call_function_1_params(done, (zval **)&retval, data);
            zval_ptr_dtor(&data);
            data = retval;
        }
    } else {
        /* 失敗時処理の実行 */
        if (NULL != fail && zend_is_callable(fail, 0, NULL TSRMLS_CC)) {
            hitsuji_call_function_1_params(fail, (zval **)&retval, data);
            zval_ptr_dtor(&data);
            data = retval;
        }
    }

    RETVAL_ZVAL(data, 1, 1);
}

#   endif       /* #ifndef HAVE_HITSUJI_CLASS_HITSUJI */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_HITSUJI_H */
