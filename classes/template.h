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

#ifndef HAVE_HITSUJI_CLASS_TEMPLATE_H
#define HAVE_HITSUJI_CLASS_TEMPLATE_H

/**
 * \hitSuji\Controllerクラス::メンバー変数定義
 */
typedef struct {
    zend_object std;
    zval *dir;
    zval *layout;
    zval *vars;
} hitsuji_template_t;

/**
 * クラスの宣言・登録部分
 */
PHP_METHOD(HSJTemplate, __construct);
PHP_METHOD(HSJTemplate, __destruct);
PHP_METHOD(HSJTemplate, layout);
PHP_METHOD(HSJTemplate, content);
PHP_METHOD(HSJTemplate, assigns);
PHP_METHOD(HSJTemplate, assign);
PHP_METHOD(HSJTemplate, display);

ZEND_BEGIN_ARG_INFO_EX(HSJTemplate_0_param, 0, ZEND_RETURN_VALUE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(HSJTemplate_1_param, 0, ZEND_RETURN_VALUE, 1)
    ZEND_ARG_INFO(0, param1)
ZEND_END_ARG_INFO()

extern zend_function_entry hitsuji_template_class_methods[];

#else
#   ifndef HAVE_HITSUJI_CLASS_TEMPLATE
#   define HAVE_HITSUJI_CLASS_TEMPLATE

/**
 * クラスの実装部分
 */
zend_function_entry hitsuji_template_class_methods[] = {
    PHP_ME(HSJTemplate, __construct,     HSJTemplate_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HSJTemplate, __destruct,      HSJTemplate_0_param, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(HSJTemplate, layout,          HSJTemplate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJTemplate, content,         HSJTemplate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJTemplate, assigns,         HSJTemplate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJTemplate, assign,          HSJTemplate_1_param, ZEND_ACC_PUBLIC)
    PHP_ME(HSJTemplate, display,         HSJTemplate_0_param, ZEND_ACC_PUBLIC)

    PHP_FE_END    /* Must be the last line in groonga_functions[] */
};

/**
 * HSJTemplateクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJTemplate, __construct)
{
    hitsuji_template_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_template_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* テンプレートファイル保存ディレクトリの取得と設定 */
    ALLOC_INIT_ZVAL(self->dir);
    ZVAL_STRING(self->dir, HITSUJI_G(template_path), 1);

    /* レイアウトファイル */
    ALLOC_INIT_ZVAL(self->layout);
    ZVAL_STRING(self->layout, "layout.tpl", 1);

    /* 変数保存用配列の初期化 */
    ALLOC_INIT_ZVAL(self->vars);
    array_init(self->vars);
}

/**
 * HSJTemplateクラス関数:コンストラクタ
 *
 * @return object
 */
PHP_METHOD(HSJTemplate, __destruct)
{
    hitsuji_template_t *self;

    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_template_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    if (NULL != self->vars) {
        zval_ptr_dtor(&self->vars);
    }

    if (NULL != self->layout) {
        zval_ptr_dtor(&self->layout);
    }

    if (NULL != self->dir) {
        zval_ptr_dtor(&self->dir);
    }
}

/**
 * layoutテンプレートの設定
 *
 * @access public
 * @param string $page ページ
 * @return void
 */
PHP_METHOD(HSJTemplate, layout)
{
    hitsuji_template_t *self;
    char *file = NULL;
    uint file_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_template_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    ZVAL_STRINGL(self->layout, file, file_len, 1);

    RETURN_CHAIN();
}

/**
 * contentテンプレートの設定
 *
 * @access public
 * @param string $content
 * @return void
 */
PHP_METHOD(HSJTemplate, content)
{
    hitsuji_template_t *self;
    char *filename;
    char *file = NULL;
    uint file_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_template_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* contentファイルのフルパス取得 */
    filename = get_filename(Z_STRVAL_P(self->dir), file);

    add_assoc_string_ex(self->vars, ZEND_STRS("content"), filename, 1);

    efree(filename);

    RETURN_CHAIN();
}

/**
 * 複数の変数を設定
 *
 * @access public
 * @param array $vars
 * @return this
 */
PHP_METHOD(HSJTemplate, assigns)
{
    hitsuji_template_t *self;
    zval *zvalues;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &zvalues) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_template_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 配列のマージ */
    zend_hash_merge(Z_ARRVAL_P(self->vars), Z_ARRVAL_P(zvalues), (copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval *), 1);

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
PHP_METHOD(HSJTemplate, assign)
{
    hitsuji_template_t *self;
    zval *zvalue;
    char *key = NULL;
    uint key_len;

    /* 引数の受け取り */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sz", &key, &key_len, &zvalue) == FAILURE) {
        RETURN_FALSE;
    }
    self = (hitsuji_template_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* 配列の追加 */
    switch (Z_TYPE_P(zvalue)) {
    case IS_NULL:
        add_assoc_null_ex(self->vars, key, key_len + 1);
        break;
    case IS_LONG:
    case IS_RESOURCE:
        add_assoc_long_ex(self->vars, key, key_len + 1, Z_LVAL_P(zvalue));
        break;
    case IS_DOUBLE:
        add_assoc_double_ex(self->vars, key, key_len + 1, Z_DVAL_P(zvalue));
        break;
    case IS_BOOL:
        add_assoc_bool_ex(self->vars, key, key_len + 1, Z_LVAL_P(zvalue));
        break;
    case IS_ARRAY:
    case IS_OBJECT:
        add_assoc_zval_ex(self->vars, key, key_len + 1, zvalue);
        break;
    case IS_STRING:
        add_assoc_string_ex(self->vars, key, key_len + 1, Z_STRVAL_P(zvalue), 1);
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
PHP_METHOD(HSJTemplate, display)
{
    hitsuji_template_t *self;
    zend_file_handle file_handle;
    char *filename;

    /* 引数の受け取り */
    if (zend_parse_parameters_none() != SUCCESS) {
        RETURN_FALSE;
    }
    self = (hitsuji_template_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

    /* extract() PHP関数の実行 */
    zend_call_method_with_1_params(NULL, NULL, NULL, "extract", NULL, self->vars);

    /* レイアウトのフルパスを取得 */
    filename = get_filename(Z_STRVAL_P(self->dir), Z_STRVAL_P(self->layout));

    /* 外部ファイルの読み込み */
    if (php_stream_open_for_zend_ex(filename, &file_handle, USE_PATH | STREAM_OPEN_FOR_INCLUDE TSRMLS_CC) == SUCCESS) {
        zend_try {
            /* 実行前の状態を退避 */
            zend_op **orig_opline_ptr = EG(opline_ptr);

            zend_execute_scripts(ZEND_INCLUDE TSRMLS_CC, NULL, 1, &file_handle);

            /* 実行前の状態を復帰 */
            EG(opline_ptr) = orig_opline_ptr;
        }
        zend_end_try();

        /* ファイルハンドラーの開放 */
        zend_destroy_file_handle(&file_handle TSRMLS_CC);
    }
    efree(filename);

    RETURN_CHAIN();
}


#   endif       /* #ifndef HAVE_HITSUJI_CLASS_TEMPLATE */
#endif      /* #ifndef HAVE_HITSUJI_CLASS_TEMPLATE_H */
