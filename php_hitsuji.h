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
 * @copyright       Copyright (c) 2011-2014 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

#ifndef HAVE_PHP_HITSUJI_H
#define HAVE_PHP_HITSUJI_H

#define PHP_HITSUJI_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#   define PHP_HITSUJI_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#   define PHP_HITSUJI_API __attribute__ ((visibility("default")))
#else
#   define PHP_HITSUJI_API
#endif

#ifdef ZTS
#   include "TSRM.h"
#endif

extern zend_class_entry *hitsuji_ce;
extern zend_class_entry *hitsuji_router_ce;
extern zend_class_entry *hitsuji_request_ce;
extern zend_class_entry *hitsuji_delegate_ce;
extern zend_class_entry *hitsuji_view_ce;
extern zend_class_entry *hitsuji_exception_ce;

extern zval *hitsuji_object_ptr;


extern zend_module_entry hitsuji_module_entry;
#define phpext_hitsuji_ptr &hitsuji_module_entry

PHP_MINIT_FUNCTION(hitSuji);
PHP_MSHUTDOWN_FUNCTION(hitSuji);
PHP_RINIT_FUNCTION(hitSuji);
PHP_RSHUTDOWN_FUNCTION(hitSuji);
PHP_MINFO_FUNCTION(hitSuji);

/**
 * メンバー定数定義:定型マクロ
 */
#define REGISTER_CLASS_CONST_LONG(pce, const_name, value) zend_declare_class_constant_long(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);
#define REGISTER_CLASS_CONST_STRING(pce, const_name, value) zend_declare_class_constant_stringl(pce, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC);

/**
 * メンバー関数呼び出し:定型マクロ
 */
#define PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
#define POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_C)
#define PUSH_EO_PARAM()
#define POP_EO_PARAM()

#define CALL_METHOD_BASE(classname, name) zim_##classname##_##name

#define CALL_METHOD_HELPER(classname, name, retval, thisptr, num, param) \
    PUSH_PARAM(param); PUSH_PARAM((void*)num); \
    PUSH_EO_PARAM(); \
    CALL_METHOD_BASE(classname, name)(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
    POP_EO_PARAM(); \
    POP_PARAM(); POP_PARAM();

#define CALL_METHOD(classname, name, retval, thisptr) \
    CALL_METHOD_BASE(classname, name)(0, retval, NULL, thisptr, 0 TSRMLS_CC);

#define CALL_METHOD1(classname, name, retval, thisptr, param1) \
    ZEND_VM_STACK_GROW_IF_NEEDED(2); \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 1, param1);

#define CALL_METHOD2(classname, name, retval, thisptr, param1, param2) \
    ZEND_VM_STACK_GROW_IF_NEEDED(3); \
    PUSH_PARAM(param1); \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 2, param2); \
    POP_PARAM();

#define CALL_METHOD3(classname, name, retval, thisptr, param1, param2, param3) \
    ZEND_VM_STACK_GROW_IF_NEEDED(4); \
    PUSH_PARAM(param1); PUSH_PARAM(param2); \
    CALL_METHOD_HELPER(classname, name, retval, thisptr, 3, param3); \
    POP_PARAM(); POP_PARAM();

/**
 * 自身のオブジェクトを返すマクロ
 */
#define RETURN_CHAIN() do {\
    Z_SET_ISREF_P(return_value); \
    Z_TYPE_P(return_value) = IS_OBJECT; \
    ZVAL_ZVAL(return_value, getThis(), 1, 0);\
} while(0)


#ifdef ZTS
#   define HITSUJI_G(v) TSRMG(hitsuji_globals_id, zend_hitsuji_globals *, v)
#else
#   define HITSUJI_G(v) (hitsuji_globals.v)
#endif


#ifndef HAVE_HITSUJI_CLASS_HITSUJI_H
#   include "classes/hitsuji.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_ROUTER_H
#   include "classes/router.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_REQUEST_H
#   include "classes/request.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_VIEW_H
#   include "classes/view.h"
#endif

#ifndef HAVE_HITSUJI_CLASS_DELEGATE_H
#   include "classes/delegate.h"
#endif

#endif    /* HAVE_PHP_HITSUJI_H */

