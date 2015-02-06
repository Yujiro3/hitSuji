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

#ifndef HAVE_HITSUJI_CALLABLE
#define HAVE_HITSUJI_CALLABLE

#include "php.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_CALLABLE_H
#   include "src/callable.h"
#endif

/**
 * 関数の実行 引数0
 *
 * @access public
 * @param zval *callable
 * @param zval *retval_ptr
 * @return int
 */
int hitsuji_call_function_0_params(zval *callable, zval *retval_ptr) 
{
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;

    int result = zend_fcall_info_init(
        callable,
        0, 
        &callback_info, 
        &callback_cache, 
        NULL, 
        NULL TSRMLS_CC
    );

    if (SUCCESS == result) {
        zend_fcall_info_args_clear(&callback_info, 1);

        callback_info.retval_ptr_ptr = &retval_ptr;
        result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
    }

    return result;
}

/**
 * 関数の実行 引数１
 *
 * @access public
 * @param zval *callable
 * @param zval *retval_ptr
 * @param zval *param
 * @return int
 */
int hitsuji_call_function_1_params(zval *callable, zval *retval_ptr, zval *param) 
{
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;

    int result = zend_fcall_info_init(
        callable,
        0, 
        &callback_info, 
        &callback_cache, 
        NULL, 
        NULL TSRMLS_CC
    );

    if (SUCCESS == result) {
        zend_fcall_info_args_clear(&callback_info, 1);
        callback_info.param_count = 1;
        callback_info.params = (zval ***) erealloc(callback_info.params, sizeof(zval **));
        *callback_info.params = &param;

        callback_info.retval_ptr_ptr = &retval_ptr;
        result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
        efree(callback_info.params);
    }
    return result;
}

/**
 * 関数の実行 引数2
 *
 * @access public
 * @param zval *callable
 * @param zval *retval_ptr
 * @param zval *param
 * @param zval *param
 * @return int
 */
int hitsuji_call_function_2_params(zval *callable, zval *retval_ptr, zval *param1, zval *param2) 
{
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;

    int result = zend_fcall_info_init(
        callable,
        0, 
        &callback_info, 
        &callback_cache, 
        NULL, 
        NULL TSRMLS_CC
    );

    if (SUCCESS == result) {
        zval ***params;

        zend_fcall_info_args_clear(&callback_info, 1);
        callback_info.param_count = 2;
        callback_info.params = params = (zval ***) erealloc(callback_info.params, 2 * sizeof(zval **));

        *params++ = &param1;
        *params   = &param2;

        callback_info.retval_ptr_ptr = &retval_ptr;
        result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
        efree(callback_info.params);
    }

    return result;
}

/**
 * ユーザー関数の実行 引数0
 *
 * @access public
 * @param const char *funcname
 * @param zval       *retval_ptr
 * @return int
 */
void hitsuji_call_user_function_0_params(const char *funcname, zval *retval_ptr) 
{
    zval zfuncname;

    ZVAL_STRING(&zfuncname, funcname, 1);
    call_user_function(
        EG(function_table), 
        NULL, 
        &zfuncname, 
        retval_ptr,      
        0, 
        NULL TSRMLS_CC
    );
}

/**
 * ユーザー関数の実行 引数1
 *
 * @access public
 * @param const char *funcname
 * @param zval       *retval_ptr
 * @param zval       *param
 * @return int
 */
void hitsuji_call_user_function_1_params(const char *funcname, zval *retval_ptr, zval *param) 
{
    zval zfuncname;
    zval *args[] = {param};

    ZVAL_STRING(&zfuncname, funcname, 1);
    call_user_function(
        EG(function_table), 
        NULL, 
        &zfuncname, 
        retval_ptr,      
        1, 
        args TSRMLS_CC
    );
}

/**
 * 指定スクリプトの実行
 *
 * @access public
 * @param const char *filename
 * @return void
 */
void hitsuji_execute_scripts_0_params(const char *filename)
{
    zend_file_handle file_handle;

    /* 外部ファイルの読み込み */
    if (php_stream_open_for_zend_ex(filename, &file_handle, USE_PATH | STREAM_OPEN_FOR_INCLUDE TSRMLS_CC) == SUCCESS) {
        zend_try {
            /* 実行前の状態を退避 */
            zend_op **orig_opline_ptr = EG(opline_ptr);

            zend_execute_scripts(ZEND_INCLUDE_ONCE TSRMLS_CC, NULL, 1, &file_handle);

            /* 実行前の状態を復帰 */
            EG(opline_ptr) = orig_opline_ptr;
        }
        zend_end_try();

        /* ファイルハンドラーの開放 */
        zend_destroy_file_handle(&file_handle TSRMLS_CC);
    }
}

/**
 * 指定スクリプトの実行 引数1
 *
 * @access public
 * @param const char *filename
 * @param zval       *param
 * @return void
 */
void hitsuji_execute_scripts_1_params(const char *filename, zval *param)
{
    zend_file_handle file_handle;
    zval retval;

    /* extract() PHP関数の実行 */
    hitsuji_call_user_function_1_params("extract", &retval, param);

    /* 外部ファイルの読み込み */
    if (php_stream_open_for_zend_ex(filename, &file_handle, USE_PATH | STREAM_OPEN_FOR_INCLUDE TSRMLS_CC) == SUCCESS) {
        zend_try {
            /* 実行前の状態を退避 */
            zend_op **orig_opline_ptr = EG(opline_ptr);

            zend_execute_scripts(ZEND_INCLUDE_ONCE TSRMLS_CC, NULL, 1, &file_handle);

            /* 実行前の状態を復帰 */
            EG(opline_ptr) = orig_opline_ptr;
        }
        zend_end_try();

        /* ファイルハンドラーの開放 */
        zend_destroy_file_handle(&file_handle TSRMLS_CC);
    }
}

/**
 * 指定スクリプトの実行 引数1
 *
 * @access public
 * @param const char *filename
 * @param HashTable  *ht
 * @return void
 */
void hitsuji_execute_scripts_1_hash(const char *filename, HashTable *ht)
{
    zval param;
    param.value.ht = ht;

    hitsuji_execute_scripts_1_params(filename, &param);
}

#endif      // #ifndef HAVE_HITSUJI_CALLABLE

