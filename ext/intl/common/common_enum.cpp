/*
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Gustavo Lopes <cataphract@php.net>                          |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "../intl_cppshims.h"

// Fix build on Windows/old versions of ICU
#include <stdio.h>

#include "common_enum.h"
#include "common_arginfo.h"

extern "C" {
#include <zend_interfaces.h>
#include <zend_exceptions.h>
}

zend_class_entry *IntlIterator_ce_ptr;
zend_object_handlers IntlIterator_handlers;

void zoi_with_current_dtor(zend_object_iterator *iter)
{
	zoi_with_current *zoiwc = (zoi_with_current*)iter;
	zval_ptr_dtor(&zoiwc->wrapping_obj);
	ZVAL_UNDEF(&zoiwc->wrapping_obj);
}

U_CFUNC zend_result zoi_with_current_valid(zend_object_iterator *iter)
{
	return Z_ISUNDEF(((zoi_with_current*)iter)->current)? FAILURE : SUCCESS;
}

U_CFUNC zval *zoi_with_current_get_current_data(zend_object_iterator *iter)
{
	return &((zoi_with_current*)iter)->current;
}

U_CFUNC void zoi_with_current_invalidate_current(zend_object_iterator *iter)
{
	zoi_with_current *zoi_iter = (zoi_with_current*)iter;
	if (!Z_ISUNDEF(zoi_iter->current)) {
		zval_ptr_dtor(&zoi_iter->current);
		ZVAL_UNDEF(&zoi_iter->current); //valid would return FAILURE now
	}
}

static void string_enum_current_move_forward(zend_object_iterator *iter)
{
	zoi_with_current *zoi_iter = (zoi_with_current*)iter;
	INTLITERATOR_METHOD_INIT_VARS;

	iter->funcs->invalidate_current(iter);

	object = &zoi_iter->wrapping_obj;
	INTLITERATOR_METHOD_FETCH_OBJECT_NO_CHECK;

	int32_t result_length;
	const char *result = ((StringEnumeration*)Z_PTR(iter->data))->next(
		&result_length, INTLITERATOR_ERROR_CODE(ii));

	intl_error_set_code(NULL, INTLITERATOR_ERROR_CODE(ii));
	if (U_FAILURE(INTLITERATOR_ERROR_CODE(ii))) {
		intl_errors_set_custom_msg(INTL_DATA_ERROR_P(ii), "Error fetching next iteration element");
	} else if (result) {
		ZVAL_STRINGL(&zoi_iter->current, result, result_length);
	} //else we've reached the end of the enum, nothing more is required
}

HashTable *zoi_with_current_get_gc(zend_object_iterator *iter, zval **table, int *n)
{
	zoi_with_current *zoiwc = reinterpret_cast<zoi_with_current*>(iter);
	*table = &zoiwc->wrapping_obj;
	*n = 1;
	return nullptr;
}

static void string_enum_rewind(zend_object_iterator *iter)
{
	zoi_with_current *zoi_iter = (zoi_with_current*)iter;
	INTLITERATOR_METHOD_INIT_VARS;

	if (!Z_ISUNDEF(zoi_iter->current)) {
		iter->funcs->invalidate_current(iter);
	}

	object = &zoi_iter->wrapping_obj;
	INTLITERATOR_METHOD_FETCH_OBJECT_NO_CHECK;

	((StringEnumeration*)Z_PTR(iter->data))->reset(INTLITERATOR_ERROR_CODE(ii));

	intl_error_set_code(NULL, INTLITERATOR_ERROR_CODE(ii));
	if (U_FAILURE(INTLITERATOR_ERROR_CODE(ii))) {
		intl_errors_set_custom_msg(INTL_DATA_ERROR_P(ii), "Error resetting enumeration");
	} else {
		iter->funcs->move_forward(iter);
	}
}

static void string_enum_destroy_it(zend_object_iterator *iter)
{
	delete (StringEnumeration*)Z_PTR(iter->data);
}

static const zend_object_iterator_funcs string_enum_object_iterator_funcs = {
	zoi_with_current_dtor,
	zoi_with_current_valid,
	zoi_with_current_get_current_data,
	NULL,
	string_enum_current_move_forward,
	string_enum_rewind,
	zoi_with_current_invalidate_current,
	zoi_with_current_get_gc,
};

U_CFUNC void IntlIterator_from_StringEnumeration(StringEnumeration *se, zval *object)
{
	IntlIterator_object *ii;
	object_init_ex(object, IntlIterator_ce_ptr);
	ii = Z_INTL_ITERATOR_P(object);
	ii->iterator = (zend_object_iterator*)emalloc(sizeof(zoi_with_current));
	zend_iterator_init(ii->iterator);
	ZVAL_PTR(&ii->iterator->data, se);
	ii->iterator->funcs = &string_enum_object_iterator_funcs;
	ii->iterator->index = 0;
	((zoi_with_current*)ii->iterator)->destroy_it = string_enum_destroy_it;
	ZVAL_OBJ_COPY(&((zoi_with_current*)ii->iterator)->wrapping_obj, Z_OBJ_P(object));
	ZVAL_UNDEF(&((zoi_with_current*)ii->iterator)->current);
}

static void IntlIterator_objects_dtor(zend_object *object)
{
	IntlIterator_object	*ii = php_intl_iterator_fetch_object(object);
	if (ii->iterator) {
		((zoi_with_current*)ii->iterator)->destroy_it(ii->iterator);
		OBJ_RELEASE(&ii->iterator->std);
		ii->iterator = NULL;
	}
}

static void IntlIterator_objects_free(zend_object *object)
{
	IntlIterator_object	*ii = php_intl_iterator_fetch_object(object);

	intl_error_reset(INTLITERATOR_ERROR_P(ii));

	zend_object_std_dtor(&ii->zo);
}

static HashTable *IntlIterator_object_get_gc(zend_object *obj, zval **table, int *n)
{
	IntlIterator_object *ii = php_intl_iterator_fetch_object(obj);
	if (ii->iterator) {
		zend_get_gc_buffer *gc_buffer = zend_get_gc_buffer_create();
		zend_get_gc_buffer_add_obj(gc_buffer, &ii->iterator->std);
		zend_get_gc_buffer_use(gc_buffer, table, n);
	} else {
		*table = nullptr;
		*n = 0;
	}
	if (obj->properties == nullptr && obj->ce->default_properties_count == 0) {
		return nullptr;
	} else {
		return zend_std_get_properties(obj);
	}
}

static zend_object_iterator *IntlIterator_get_iterator(
	zend_class_entry *ce, zval *object, int by_ref)
{
	if (by_ref) {
		zend_throw_exception(NULL,
			"Iteration by reference is not supported", 0);
		return NULL;
	}

	IntlIterator_object *ii = Z_INTL_ITERATOR_P(object);

	if (ii->iterator == NULL) {
		zend_throw_exception(NULL,
			"The IntlIterator is not properly constructed", 0);
		return NULL;
	}

	GC_ADDREF(&ii->iterator->std);

	return ii->iterator;
}

static zend_object *IntlIterator_object_create(zend_class_entry *ce)
{
	IntlIterator_object	*intern = (IntlIterator_object*)zend_object_alloc(sizeof(IntlIterator_object), ce);

	zend_object_std_init(&intern->zo, ce);
    object_properties_init(&intern->zo, ce);
	intl_error_init(INTLITERATOR_ERROR_P(intern));

	intern->iterator = NULL;

	return &intern->zo;
}

PHP_METHOD(IntlIterator, current)
{
	zval *data;
	INTLITERATOR_METHOD_INIT_VARS;

	ZEND_PARSE_PARAMETERS_NONE();

	INTLITERATOR_METHOD_FETCH_OBJECT;
	data = ii->iterator->funcs->get_current_data(ii->iterator);
	if (data) {
		RETURN_COPY_DEREF(data);
	}
}

PHP_METHOD(IntlIterator, key)
{
	INTLITERATOR_METHOD_INIT_VARS;

	ZEND_PARSE_PARAMETERS_NONE();

	INTLITERATOR_METHOD_FETCH_OBJECT;

	if (ii->iterator->funcs->get_current_key) {
		ii->iterator->funcs->get_current_key(ii->iterator, return_value);
	} else {
		RETURN_LONG(ii->iterator->index);
	}
}

PHP_METHOD(IntlIterator, next)
{
	INTLITERATOR_METHOD_INIT_VARS;

	ZEND_PARSE_PARAMETERS_NONE();

	INTLITERATOR_METHOD_FETCH_OBJECT;
	ii->iterator->funcs->move_forward(ii->iterator);
	/* foreach also advances the index after the last iteration,
	 * so I see no problem in incrementing the index here unconditionally */
	ii->iterator->index++;
}

PHP_METHOD(IntlIterator, rewind)
{
	INTLITERATOR_METHOD_INIT_VARS;

	ZEND_PARSE_PARAMETERS_NONE();

	INTLITERATOR_METHOD_FETCH_OBJECT;
	if (ii->iterator->funcs->rewind) {
		ii->iterator->funcs->rewind(ii->iterator);
	} else {
		intl_errors_set(INTLITERATOR_ERROR_P(ii), U_UNSUPPORTED_ERROR,
			"rewind not supported");
	}
}

PHP_METHOD(IntlIterator, valid)
{
	INTLITERATOR_METHOD_INIT_VARS;

	ZEND_PARSE_PARAMETERS_NONE();

	INTLITERATOR_METHOD_FETCH_OBJECT;
	RETURN_BOOL(ii->iterator->funcs->valid(ii->iterator) == SUCCESS);
}

U_CFUNC void intl_register_common_symbols(int module_number)
{
	/* Create and register 'IntlIterator' class. */
	IntlIterator_ce_ptr = register_class_IntlIterator(zend_ce_iterator);
	IntlIterator_ce_ptr->create_object = IntlIterator_object_create;
	IntlIterator_ce_ptr->default_object_handlers = &IntlIterator_handlers;
	IntlIterator_ce_ptr->get_iterator = IntlIterator_get_iterator;

	memcpy(&IntlIterator_handlers, &std_object_handlers,
		sizeof IntlIterator_handlers);
	IntlIterator_handlers.offset = XtOffsetOf(IntlIterator_object, zo);
	IntlIterator_handlers.clone_obj = NULL;
	IntlIterator_handlers.dtor_obj = IntlIterator_objects_dtor;
	IntlIterator_handlers.free_obj = IntlIterator_objects_free;
	IntlIterator_handlers.get_gc = IntlIterator_object_get_gc;

	register_common_symbols(module_number);
}
