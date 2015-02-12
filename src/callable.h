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

#ifndef HAVE_HITSUJI_CALLABLE_H
#define HAVE_HITSUJI_CALLABLE_H

zval *hitsuji_call_function_0_params(zval *callable, zval *retval_ptr);
zval *hitsuji_call_function_1_params(zval *callable, zval *retval_ptr, zval *param);
zval *hitsuji_call_function_2_params(zval *callable, zval *retval_ptr, zval *param1, zval *param2);
zval *hitsuji_calls_function_1_params(HashTable *ht, zval *retval_ptr, zval *param);
int hitsuji_call_function_args(zval *callable, zval *retval_ptr, zval *param);

void hitsuji_call_user_function_0_params(const char *funcname, zval *retval_ptr);
void hitsuji_call_user_function_1_params(const char *funcname, zval *retval_ptr, zval *param);

void hitsuji_execute_scripts(const char *filename);

#endif  // #ifndef HAVE_HITSUJI_CALLABLE_H
