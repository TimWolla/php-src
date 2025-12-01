/* This is a generated file, edit encoding.stub.php instead.
 * Stub hash: ee4210cc6d684ab5459725a82b9773f851628a11
 * Has decl header: yes */

static zend_class_entry *register_class_Encoding_EncodingError(zend_class_entry *class_entry_Error)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Encoding", "EncodingError", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Error, 0);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_EncodingException(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Encoding", "EncodingException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Exception, 0);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_UnableToDecodeException(zend_class_entry *class_entry_Encoding_EncodingException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Encoding", "UnableToDecodeException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Encoding_EncodingException, 0);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_UnableToEncodeException(zend_class_entry *class_entry_Encoding_EncodingException)
{
	zend_class_entry ce, *class_entry;

	INIT_NS_CLASS_ENTRY(ce, "Encoding", "UnableToEncodeException", NULL);
	class_entry = zend_register_internal_class_with_flags(&ce, class_entry_Encoding_EncodingException, 0);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_Base16(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\Base16", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Upper", NULL);

	zend_enum_add_case_cstr(class_entry, "Lower", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_Base32(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\Base32", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Ascii", NULL);

	zend_enum_add_case_cstr(class_entry, "Hex", NULL);

	zend_enum_add_case_cstr(class_entry, "Crockford", NULL);

	zend_enum_add_case_cstr(class_entry, "Z", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_Base58(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\Base58", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Bitcoin", NULL);

	zend_enum_add_case_cstr(class_entry, "Flickr", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_Base64(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\Base64", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Standard", NULL);

	zend_enum_add_case_cstr(class_entry, "UrlSafe", NULL);

	zend_enum_add_case_cstr(class_entry, "Imap", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_Base85(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\Base85", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Adobe", NULL);

	zend_enum_add_case_cstr(class_entry, "Z85", NULL);

	zend_enum_add_case_cstr(class_entry, "Git", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_PaddingMode(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\PaddingMode", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "VariantControlled", NULL);

	zend_enum_add_case_cstr(class_entry, "StripPadding", NULL);

	zend_enum_add_case_cstr(class_entry, "PreservePadding", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_DecodingMode(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\DecodingMode", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Forgiving", NULL);

	zend_enum_add_case_cstr(class_entry, "Strict", NULL);

	return class_entry;
}

static zend_class_entry *register_class_Encoding_TimingMode(void)
{
	zend_class_entry *class_entry = zend_register_internal_enum("Encoding\\TimingMode", IS_UNDEF, NULL);

	zend_enum_add_case_cstr(class_entry, "Variable", NULL);

	zend_enum_add_case_cstr(class_entry, "Constant", NULL);

	return class_entry;
}
