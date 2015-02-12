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
#include "zend_interfaces.h"

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
 * @return *zval
 */
zval *hitsuji_call_function_0_params(zval *callable, zval *retval_ptr) 
{
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;
    zval *retval;

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

        callback_info.retval_ptr_ptr = retval_ptr ? &retval_ptr : &retval;
        result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
    }

    if (!retval_ptr) {
        if (retval) {
            zval_ptr_dtor(&retval);
        }
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
 * @return *zval
 */
zval *hitsuji_call_function_1_params(zval *callable, zval *retval_ptr, zval *param) 
{
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;
    zval *retval;

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
        callback_info.param_count = 1;
        callback_info.params = params = (zval ***) erealloc(callback_info.params, 1 * sizeof(zval **));

        *params = &param;

        callback_info.retval_ptr_ptr = retval_ptr ? &retval_ptr : &retval;
        result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
        efree(callback_info.params);
    }

    if (!retval_ptr) {
        if (retval) {
            zval_ptr_dtor(&retval);
        }
        return NULL;
    }

    return retval_ptr;
}

/**
 * 関数の実行 引数2
 *
 * @access public
 * @param zval *callable
 * @param zval *retval_ptr
 * @param zval *param
 * @param zval *param
 * @return *zval
 */
zval *hitsuji_call_function_2_params(zval *callable, zval *retval_ptr, zval *param1, zval *param2) 
{
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;
    zval *retval;

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

        callback_info.retval_ptr_ptr = retval_ptr ? &retval_ptr : &retval;
        result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
        efree(callback_info.params);
    }

    if (!retval_ptr) {
        if (retval) {
            zval_ptr_dtor(&retval);
        }
    }

    return result;
}

/**
 * 複数関数の実行 引数1
 *
 * @access public
 * @param HashTable *ht
 * @param zval      *retval_ptr
 * @param zval      *param
 * @return *zval
 */
zval *hitsuji_calls_function_1_params(HashTable *ht, zval *retval_ptr, zval *param)
{
    HashPosition pos;
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;
    zval *retval, **callable;

    if (zend_hash_num_elements(ht) == 0) {
        return param;
    }

    for (zend_hash_internal_pointer_reset_ex(ht, &pos);
         zend_hash_get_current_data_ex(ht, (void **)&callable, &pos) == SUCCESS;
         zend_hash_move_forward_ex(ht, &pos)
    ) {
        int result = zend_fcall_info_init(
            *callable,
            0, 
            &callback_info, 
            &callback_cache, 
            NULL, 
            NULL TSRMLS_CC
        );

        if (SUCCESS == result) {
            zval ***params;

            zend_fcall_info_args_clear(&callback_info, 1);
            callback_info.param_count = 1;
            callback_info.params = params = (zval ***) erealloc(callback_info.params, sizeof(zval **));

            *params = &param;

            callback_info.retval_ptr_ptr = &retval;
            result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
            efree(callback_info.params);
        }

        if (zend_is_true(retval)) {
            zval_ptr_dtor(&param);
            param = retval;
        }
    }

    return retval_ptr = retval;
}

/**
 * 関数の実行 引数配列
 *
 * @access public
 * @param zval *callable
 * @param zval *retval_ptr
 * @param zval *param
 * @return int
 */
int hitsuji_call_function_args(zval *callable, zval *retval_ptr, zval *param) 
{
    zend_fcall_info callback_info;
    zend_fcall_info_cache callback_cache;
    zval *retval;

    if (!param) {
        return SUCCESS;
    }

    if (Z_TYPE_P(param) != IS_ARRAY) {
        return FAILURE;
    }


    int result = zend_fcall_info_init(
        callable,
        0, 
        &callback_info, 
        &callback_cache, 
        NULL, 
        NULL TSRMLS_CC
    );

    if (SUCCESS == result) {
        HashPosition pos;
        zval **arg, ***params;

        callback_info.param_count = zend_hash_num_elements(Z_ARRVAL_P(param));
        callback_info.params = params = (zval ***) erealloc(callback_info.params, callback_info.param_count * sizeof(zval **));

        zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(param), &pos);
        while (zend_hash_get_current_data_ex(Z_ARRVAL_P(param), (void *) &arg, &pos) == SUCCESS) {
            *params++ = arg;
            zend_hash_move_forward_ex(Z_ARRVAL_P(param), &pos);
        }

        callback_info.retval_ptr_ptr = retval_ptr ? &retval_ptr : &retval;
        result = zend_call_function(&callback_info, &callback_cache TSRMLS_CC);
        efree(callback_info.params);
    }

    if (!retval_ptr) {
        if (retval) {
            zval_ptr_dtor(&retval);
        }
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
void hitsuji_execute_scripts(const char *filename)
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
#endif      // #ifndef HAVE_HITSUJI_CALLABLE

