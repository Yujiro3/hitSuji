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

#ifndef HAVE_HITSUJI_ARRAY
#define HAVE_HITSUJI_ARRAY

#include "php.h"
#include "zend_interfaces.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_ARRAY_H
#   include "src/array.h"
#endif

/**
 * 配列の数値をキーとする変数を消去
 *
 * @access public
 * @param  HashTable *array
 * @return void
 */
void hash_trim_index(HashTable *array) 
{
    HashPosition position;
    zval **operand;
    array_trim_index_t trims[zend_hash_num_elements(array)];
    int row=0, cnt = 0;

    if (zend_hash_num_elements(array) == 0) {
        return;
    }

    for (zend_hash_internal_pointer_reset_ex(array, &position);
         zend_hash_get_current_data_ex(array, (void **)&operand, &position) == SUCCESS;
         zend_hash_move_forward_ex(array, &position)
    ) {
        char *key = NULL;
        uint key_len = 0;
        ulong index;

        int key_type = zend_hash_get_current_key_ex(array, &key, &key_len, &index, 0, &position);
        trims[row].index = index;

        if (HASH_KEY_IS_LONG == key_type) {
            trims[row].flag = 1;
        } else {
            trims[row].flag = 0;
        }
        row++;
    } // for (...)

    for (cnt = 0; cnt < row; cnt++) {
        if (trims[cnt].flag) {
            zend_hash_del_key_or_index(array, NULL, 0, trims[cnt].index, HASH_DEL_INDEX);
        }
    }
    zend_hash_internal_pointer_reset(array);
}

/**
 * 配列の中身を全て消去
 *
 * @access public
 * @param  HashTable *array
 * @return void
 */
void hash_all_clean(HashTable *array) 
{
    HashPosition position;
    zval **operand;

    if (zend_hash_num_elements(array) == 0) {
        return;
    }
    zend_hash_clean(array);
}

/**
 * 配列の数値をキーとする変数を消去
 *
 * @access public
 * @param  zval *array
 * @return void
 */
void array_trim_index(zval *array) 
{
    if (IS_ARRAY != Z_TYPE_P(array)) {
        return;
    }
    hash_trim_index(Z_ARRVAL_P(array));
}

/**
 * 配列の中身を全て消去
 *
 * @access public
 * @param  zval *array
 * @return void
 */
void array_all_clean(zval *array) 
{
    if (IS_ARRAY != Z_TYPE_P(array)) {
        return;
    }
    hash_all_clean(Z_ARRVAL_P(array));
}

/**
 * 変数を生成して変数のコピー
 *
 * @access public
 * @param zval *source コピー元
 * @return *zval
 */
zval *array_alloc_copy(zval *source, int dtor)
{
    zval *dest = NULL;
    ALLOC_INIT_ZVAL(dest);

    switch (Z_TYPE_P(source)) {
    case IS_LONG:
    case IS_RESOURCE:
        ZVAL_LONG(dest, Z_LVAL_P(source));
        break;
    case IS_DOUBLE:
        ZVAL_DOUBLE(dest, Z_DVAL_P(source));
        break;
    case IS_BOOL:
        ZVAL_BOOL(dest, Z_LVAL_P(source));
        break;
    case IS_STRING:
        ZVAL_STRING(dest, Z_STRVAL_P(source), 1);
        break;
    case IS_ARRAY:
    case IS_OBJECT:
        ZVAL_ZVAL(dest, source, 1, 0);
        break;
    default :
        return source;
    }

    if (dtor) {
        zval_ptr_dtor(&source);
    }

    return dest;
}

/**
 * 配列をbool値とデータに分離
 *
 * @access public
 * @param  int  *result
 * @param  zval *array
 * @return int
 */
zval *array_bool_data(int *result, zval *array) 
{
    HashPosition position;
    zval **data, *retval;
    char *key = NULL;
    uint key_len = 0;
    ulong index;

    *result = 0;
    if (!zend_is_true(array)) {
        return array_alloc_copy(array, 1);
    }

    if (IS_ARRAY != Z_TYPE_P(array)) {
        *result = 1;
        return array_alloc_copy(array, 1);
    }

    if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
        return array_alloc_copy(array, 1);
    }

    /* 先頭の配列をチェック */
    zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &position);
    if (zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&data, &position) == SUCCESS) {
        if (IS_BOOL == Z_TYPE_PP(data)) {
            if (zend_is_true(*data)) {
                *result = 1;
            }
        } else {
            *result = 1;
            return array_alloc_copy(array, 1);
        }
    }

    /* 次の配列へ進める */
    zend_hash_move_forward_ex(Z_ARRVAL_P(array), &position);

    if (zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&data, &position) != SUCCESS) {
        *result = 1;
        return array_alloc_copy(array, 1);
    }
    retval = array_alloc_copy(*data, 0);
    zval_ptr_dtor(&array);

    return retval;
}

#endif      // #ifndef HAVE_HITSUJI_ARRAY

