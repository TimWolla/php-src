/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 07475caecc81ab3b38a04905f874615af1126289 */

static const zend_function_entry * const spl_exceptions_functions = NULL;

static const zend_function_entry * const class_LogicException_methods = NULL;

static const zend_function_entry * const class_BadFunctionCallException_methods = NULL;

static const zend_function_entry * const class_BadMethodCallException_methods = NULL;

static const zend_function_entry * const class_DomainException_methods = NULL;

static const zend_function_entry * const class_InvalidArgumentException_methods = NULL;

static const zend_function_entry * const class_LengthException_methods = NULL;

static const zend_function_entry * const class_OutOfRangeException_methods = NULL;

static const zend_function_entry * const class_RuntimeException_methods = NULL;

static const zend_function_entry * const class_OutOfBoundsException_methods = NULL;

static const zend_function_entry * const class_OverflowException_methods = NULL;

static const zend_function_entry * const class_RangeException_methods = NULL;

static const zend_function_entry * const class_UnderflowException_methods = NULL;

static const zend_function_entry * const class_UnexpectedValueException_methods = NULL;

static zend_class_entry *register_class_LogicException(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "LogicException", class_LogicException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Exception, 0);

	return class_entry;
}

static zend_class_entry *register_class_BadFunctionCallException(zend_class_entry *class_entry_LogicException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "BadFunctionCallException", class_BadFunctionCallException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_LogicException, 0);

	return class_entry;
}

static zend_class_entry *register_class_BadMethodCallException(zend_class_entry *class_entry_BadFunctionCallException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "BadMethodCallException", class_BadMethodCallException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_BadFunctionCallException, 0);

	return class_entry;
}

static zend_class_entry *register_class_DomainException(zend_class_entry *class_entry_LogicException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "DomainException", class_DomainException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_LogicException, 0);

	return class_entry;
}

static zend_class_entry *register_class_InvalidArgumentException(zend_class_entry *class_entry_LogicException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "InvalidArgumentException", class_InvalidArgumentException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_LogicException, 0);

	return class_entry;
}

static zend_class_entry *register_class_LengthException(zend_class_entry *class_entry_LogicException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "LengthException", class_LengthException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_LogicException, 0);

	return class_entry;
}

static zend_class_entry *register_class_OutOfRangeException(zend_class_entry *class_entry_LogicException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "OutOfRangeException", class_OutOfRangeException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_LogicException, 0);

	return class_entry;
}

static zend_class_entry *register_class_RuntimeException(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "RuntimeException", class_RuntimeException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Exception, 0);

	return class_entry;
}

static zend_class_entry *register_class_OutOfBoundsException(zend_class_entry *class_entry_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "OutOfBoundsException", class_OutOfBoundsException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_RuntimeException, 0);

	return class_entry;
}

static zend_class_entry *register_class_OverflowException(zend_class_entry *class_entry_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "OverflowException", class_OverflowException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_RuntimeException, 0);

	return class_entry;
}

static zend_class_entry *register_class_RangeException(zend_class_entry *class_entry_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "RangeException", class_RangeException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_RuntimeException, 0);

	return class_entry;
}

static zend_class_entry *register_class_UnderflowException(zend_class_entry *class_entry_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "UnderflowException", class_UnderflowException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_RuntimeException, 0);

	return class_entry;
}

static zend_class_entry *register_class_UnexpectedValueException(zend_class_entry *class_entry_RuntimeException)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "UnexpectedValueException", class_UnexpectedValueException_methods);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_RuntimeException, 0);

	return class_entry;
}
