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
 * @param zval *array
 * @return void
 */
void hash_trim_index(HashTable *array) 
{
    HashPosition position;
    zval **operand;

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

        if (HASH_KEY_IS_LONG == key_type) {
            zend_hash_del_key_or_index(array, NULL, 0, index, HASH_DEL_INDEX);
        }
    } // for (...)
}

/**
 * 配列の中身を全て消去
 *
 * @access public
 * @param zval *array
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
 * 配列をbool値とデータに分離
 *
 * @access public
 * @param zval *array
 * @return int
 */
int hash_bool_data(HashTable *array, zval *value) 
{
    HashPosition position;
    zval **data;
    int result = 0;
    char *key = NULL;
    uint key_len = 0;
    ulong index;

    if (zend_hash_num_elements(array) == 0) {
        return result;
    }

    /* 先頭の配列をチェック */
    zend_hash_internal_pointer_reset_ex(array, &position);
    if (zend_hash_get_current_data_ex(array, (void **)&data, &position) == SUCCESS) {
        if (IS_BOOL == Z_TYPE_PP(data)) {
            if (zend_is_true(*data)) {
                result = 1;
            }
        } else {
            return 1;
        }
    }

    /* 次の配列へ進める */
    zend_hash_move_forward_ex(array, &position);

    if (zend_hash_get_current_data_ex(array, (void **)&data, &position) != SUCCESS) {
        return result;
    }
    ZVAL_ZVAL(value, *data, 1, 0);

    return result;
}

/**
 * 配列の数値をキーとする変数を消去
 *
 * @access public
 * @param zval *array
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
 * @param zval *array
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
 * 配列をbool値とデータに分離
 *
 * @access public
 * @param zval *array
 * @return int
 */
int array_bool_data(zval *array, zval *value) 
{
    if (!zend_is_true(array)) {
        return 0;
    }

    if (IS_ARRAY != Z_TYPE_P(array)) {
        return 1;
    }

    return hash_bool_data(Z_ARRVAL_P(array), value);
}

#endif      // #ifndef HAVE_HITSUJI_ARRAY

