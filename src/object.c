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

#ifndef HAVE_HITSUJI_OBJECT
#define HAVE_HITSUJI_OBJECT

#include "php.h"

#ifndef HAVE_PHP_HITSUJI_H
#   include "php_hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_OBJECT_H
#   include "src/object.h"
#endif

/**
 * hitSujiクラスのデストラクタ (メモリ解放)
 *
 * @param hitsuji_t *self 開放する構造体
 * @return void
 */
static void hitsuji_dtor(hitsuji_t *self TSRMLS_DC)
{
    /* カラムリストの開放 */
    zend_object_std_dtor(&self->std TSRMLS_CC);
    efree(self);
}

/**
 * hitSujiクラスのコンストラクタ (メモリ確保)
 *
 * @param zend_class_entry *ce クラスエントリ
 * @return zend_object_value
 */
zend_object_value hitsuji_ctor(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    hitsuji_t *self;

    self = ecalloc(1, sizeof(*self));

    zend_object_std_init(&self->std, ce TSRMLS_CC);
    object_properties_init(&self->std, ce);
    rebuild_object_properties(&self->std);

    retval.handle = zend_objects_store_put(
        self, 
        (zend_objects_store_dtor_t)zend_objects_destroy_object, 
        (zend_objects_free_object_storage_t)hitsuji_dtor, 
        NULL TSRMLS_CC
    );
    retval.handlers = zend_get_std_object_handlers();

    return retval;
}

/**
 * routerクラスのデストラクタ (メモリ解放)
 *
 * @param hitsuji_router_t *self 開放する構造体
 * @return void
 */
static void hitsuji_router_dtor(hitsuji_router_t *self TSRMLS_DC)
{
    /* カラムリストの開放 */
    zend_object_std_dtor(&self->std TSRMLS_CC);
    efree(self);
}

/**
 * routerクラスのコンストラクタ (メモリ確保)
 *
 * @param zend_class_entry *ce クラスエントリ
 * @return zend_object_value
 */
zend_object_value hitsuji_router_ctor(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    hitsuji_router_t *self;

    self = ecalloc(1, sizeof(*self));

    zend_object_std_init(&self->std, ce TSRMLS_CC);
    object_properties_init(&self->std, ce);
    rebuild_object_properties(&self->std);

    self->routes      = NULL;
    self->callable    = NULL;
    self->is_callable = 0;

    retval.handle = zend_objects_store_put(
        self, 
        (zend_objects_store_dtor_t)zend_objects_destroy_object, 
        (zend_objects_free_object_storage_t)hitsuji_router_dtor, 
        NULL TSRMLS_CC
    );
    retval.handlers = zend_get_std_object_handlers();

    return retval;
}

/**
 * viewクラスのデストラクタ (メモリ解放)
 *
 * @param hitsuji_view_t *self 開放する構造体
 * @return void
 */
static void hitsuji_view_dtor(hitsuji_view_t *self TSRMLS_DC)
{
    /* カラムリストの開放 */
    zend_object_std_dtor(&self->std TSRMLS_CC);
    efree(self);
}

/**
 * viewクラスのコンストラクタ (メモリ確保)
 *
 * @param zend_class_entry *ce クラスエントリ
 * @return zend_object_value
 */
zend_object_value hitsuji_view_ctor(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    hitsuji_view_t *self;

    self = ecalloc(1, sizeof(*self));

    zend_object_std_init(&self->std, ce TSRMLS_CC);
    object_properties_init(&self->std, ce);
    rebuild_object_properties(&self->std);

    retval.handle = zend_objects_store_put(
        self, 
        (zend_objects_store_dtor_t)zend_objects_destroy_object, 
        (zend_objects_free_object_storage_t)hitsuji_view_dtor, 
        NULL TSRMLS_CC
    );
    retval.handlers = zend_get_std_object_handlers();

    return retval;
}

#endif      // #ifndef HAVE_HITSUJI_OBJECT

