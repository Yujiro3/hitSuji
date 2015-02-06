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

#ifndef HAVE_HITSUJI_CLASS_VIEW_H
#define HAVE_HITSUJI_CLASS_VIEW_H

/**
 * \hitSuji\Controllerクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
} hitsuji_view_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(HSJView, __construct);
PHP_METHOD(HSJView, __destruct);
PHP_METHOD(HSJView, layout);
PHP_METHOD(HSJView, content);
PHP_METHOD(HSJView, assigns);
PHP_METHOD(HSJView, assign);
PHP_METHOD(HSJView, display);

ZEND_BEGIN_ARG_INFO_EX(HSJView_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJView_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, param1)
ZEND_END_ARG_INFO()

extern zend_function_entry hitsuji_view_class_methods[];

#else
#   ifndef HAVE_HITSUJI_CLASS_VIEW
#   define HAVE_HITSUJI_CLASS_VIEW

/**
 * クラスの実装部分
 */
zend_function_entry hitsuji_view_class_methods[] = {
    PHP_ME(HSJView, __construct,     HSJView_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HSJView, __destruct,      HSJView_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HSJView, layout,          HSJView_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJView, content,         HSJView_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJView, assigns,         HSJView_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJView, assign,          HSJView_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJView, display,         HSJView_0_param, ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * HSJViewクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJView, __construct)
{
    zval *zvars, zdir, zkey;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    /* テンプレートファイル保存ディレクトリの取得と設定 */
    ZVAL_STRING(&zkey, "tpl", 1);
    CALL_METHOD1(hitSuji, dir, &zdir, hitsuji_object_ptr, &zkey);
    zend_update_property_string(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("dir"), Z_STRVAL(zdir) TSRMLS_CC);

    /* レイアウトファイル */
    zend_update_property_string(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("layout"), "layout.tpl" TSRMLS_CC);

    /* 変数保存用配列の初期化 */
    zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);
    array_init(zvars);
}

/**
 * HSJViewクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJView, __destruct)
{
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

}

/**
 * layoutテンプレートの設定
 *
 * @access public
 * @param string $page ページ
 * @return void
 */
PHP_METHOD(HSJView, layout)
{
    char *file = NULL;
    uint file_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
        RETURN_FALSE;
    }
    zend_update_property_string(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("layout"), file TSRMLS_CC);

    RETURN_CHAIN();
}

/**
 * contentテンプレートの設定
 *
 * @access public
 * @param string $content
 * @return void
 */
PHP_METHOD(HSJView, content)
{
    zval *zvars, *zdir, zkey;
    char *file = NULL;
    uint file_len;
    char filename[256];

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
        RETURN_FALSE;
    }
    zdir = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("dir"), 1 TSRMLS_CC);

    /* ディレクトリパスの追加 */
    strcpy(filename, Z_STRVAL_P(zdir));

    /* contentファイル名の追加 */
    strcat(filename, file);

    zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);
    add_assoc_string_ex(zvars, ZEND_STRS("content"), filename, 1);

    RETURN_CHAIN();
}

/**
 * 複数の変数を設定
 *
 * @access public
 * @param array $vars
 * @return this
 */
PHP_METHOD(HSJView, assigns)
{
    zval *zvars, *zvalues;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &zvalues) == FAILURE) {
        RETURN_FALSE;
    }

    /* 配列のマージ */
    zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);
    php_array_merge(Z_ARRVAL_P(zvalues), Z_ARRVAL_P(zvars), 0 TSRMLS_CC);

    RETURN_CHAIN();
}

/**
 * 変数を設定
 *
 * @access public
 * @param string $key
 * @param mixed $value
 * @return this
 */
PHP_METHOD(HSJView, assign)
{
    zval *zvars, *zvalue;
    char *key = NULL;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &key, &key_len, &zvalue) == FAILURE) {
        RETURN_FALSE;
    }

    /* 配列の追加 */
    zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);

    switch (Z_TYPE_P(zvalue)) {
    case IS_NULL:
        add_assoc_null_ex(zvars, key, key_len + 1);
        break;
    case IS_LONG:
    case IS_RESOURCE:
        add_assoc_long_ex(zvars, key, key_len + 1, Z_LVAL_P(zvalue));
        break;
    case IS_DOUBLE:
        add_assoc_double_ex(zvars, key, key_len + 1, Z_DVAL_P(zvalue));
        break;
    case IS_BOOL:
        add_assoc_bool_ex(zvars, key, key_len + 1, Z_LVAL_P(zvalue));
        break;
    case IS_ARRAY:
    case IS_OBJECT:
        add_assoc_zval_ex(zvars, key, key_len + 1, zvalue);
        break;
    case IS_STRING:
        add_assoc_string_ex(zvars, key, key_len + 1, Z_STRVAL_P(zvalue), 1);
        break;
    }

    RETURN_CHAIN();
}

/**
 * layoutテンプレートの設定
 *
 * @access public
 * @return void
 */
PHP_METHOD(HSJView, display)
{
    zval *zdir, *zlayout, retval, *zvars;
    zend_file_handle file_handle;
    char filename[256];

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }

    zvars = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("vars"), 1 TSRMLS_CC);

    /* extract() PHP関数の実行 */
    hitsuji_call_user_function_1_params("extract", &retval, zvars);

    /* ディレクトリパスの追加 */
    zdir = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("dir"), 1 TSRMLS_CC);
    strcpy(filename, Z_STRVAL_P(zdir));

    /* レイアウトファイル名の追加 */
    zlayout = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("layout"), 1 TSRMLS_CC);
    strcat(filename, Z_STRVAL_P(zlayout));

    /* 外部ファイルの読み込み */
    if (php_stream_open_for_zend_ex(filename, &file_handle, USE_PATH | STREAM_OPEN_FOR_INCLUDE TSRMLS_CC) == SUCCESS) {
        zend_try {
            /* 実行前の状態を退避 */
            zend_op **orig_opline_ptr = EG(opline_ptr);

            zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC, return_value_ptr, 1, &file_handle);

            /* 実行前の状態を復帰 */
            EG(opline_ptr) = orig_opline_ptr;
        }
        zend_end_try();

        /* ファイルハンドラーの開放 */
        zend_destroy_file_handle(&file_handle TSRMLS_CC);
    }

    RETURN_CHAIN();
}

#   endif       /* #ifndef HAVE_HITSUJI_CLASS_VIEW */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_VIEW_H */
