/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Tim Düsterhus <timwolla@php.net>                            |
   +----------------------------------------------------------------------+
*/

#include "php.h"
#include "Zend/zend_enum.h"
#include "Zend/zend_exceptions.h"
#include "encoding_arginfo.h"

PHPAPI zend_class_entry *encoding_ce_EncodingError;
PHPAPI zend_class_entry *encoding_ce_EncodingException;
PHPAPI zend_class_entry *encoding_ce_UnableToDecodeException;
PHPAPI zend_class_entry *encoding_ce_UnableToEncodeException;
PHPAPI zend_class_entry *encoding_ce_Base16;
PHPAPI zend_class_entry *encoding_ce_Base32;
PHPAPI zend_class_entry *encoding_ce_Base58;
PHPAPI zend_class_entry *encoding_ce_Base64;
PHPAPI zend_class_entry *encoding_ce_Base85;
PHPAPI zend_class_entry *encoding_ce_PaddingMode;
PHPAPI zend_class_entry *encoding_ce_DecodingMode;
PHPAPI zend_class_entry *encoding_ce_TimingMode;

PHP_MINIT_FUNCTION(encoding)
{
	encoding_ce_EncodingError = register_class_Encoding_EncodingError(zend_ce_error);
	encoding_ce_EncodingException = register_class_Encoding_EncodingException(zend_ce_exception);
	encoding_ce_UnableToDecodeException = register_class_Encoding_UnableToDecodeException(encoding_ce_EncodingException);
	encoding_ce_UnableToEncodeException = register_class_Encoding_UnableToEncodeException(encoding_ce_EncodingException);
	encoding_ce_Base16 = register_class_Encoding_Base16();
	encoding_ce_Base32 = register_class_Encoding_Base32();
	encoding_ce_Base58 = register_class_Encoding_Base58();
	encoding_ce_Base64 = register_class_Encoding_Base64();
	encoding_ce_Base85 = register_class_Encoding_Base85();
	encoding_ce_PaddingMode = register_class_Encoding_PaddingMode();
	encoding_ce_DecodingMode = register_class_Encoding_DecodingMode();
	encoding_ce_TimingMode = register_class_Encoding_TimingMode();

	return SUCCESS;
}

static bool is_constant_time(zend_object *timing_mode, bool default_mode)
{
	if (timing_mode) {
		return zend_string_equals_literal(Z_STR_P(zend_enum_fetch_case_name(timing_mode)), "Constant");
	}

	return default_mode;
}

PHP_FUNCTION(Encoding_base16_encode)
{
	zend_string *data;
	zend_object *variant_obj = NULL;
	zend_object *timing_mode = NULL;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJ_OF_CLASS(variant_obj, encoding_ce_Base16);
		Z_PARAM_OBJ_OF_CLASS(timing_mode, encoding_ce_TimingMode);
	ZEND_PARSE_PARAMETERS_END();

	const char *variant_alphabet = "0123456789ABCDEF";

	if (variant_obj) {
		zend_string *variant_name = Z_STR_P(zend_enum_fetch_case_name(variant_obj));
		if (zend_string_equals_literal(variant_name, "Lower")) {
			variant_alphabet = "0123456789abcdef";
		}
	}

	if (is_constant_time(timing_mode, /* default */ false)) {
		zend_throw_error(zend_ce_error, "Not implemented");
		RETURN_THROWS();
	} else {
		zend_string *result = zend_string_safe_alloc(ZSTR_LEN(data), 2 * sizeof(char), 0, 0);

		char *target = ZSTR_VAL(result);
		for (size_t i = 0; i < ZSTR_LEN(data); i++) {
			unsigned char current = ZSTR_VAL(data)[i];
			*target++ = variant_alphabet[current >> 4];
			*target++ = variant_alphabet[current & 0xf];
		}
		*target = '\0';

		RETURN_NEW_STR(result);
	}
}

PHP_FUNCTION(Encoding_base16_decode)
{
	zend_string *data;
	zend_object *variant_obj = NULL;
	zend_object *decoding_mode = NULL;
	zend_object *timing_mode = NULL;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJ_OF_CLASS(variant_obj, encoding_ce_Base16);
		Z_PARAM_OBJ_OF_CLASS(decoding_mode, encoding_ce_DecodingMode);
		Z_PARAM_OBJ_OF_CLASS(timing_mode, encoding_ce_TimingMode);
	ZEND_PARSE_PARAMETERS_END();

	const char *variant_alphabet = "0123456789ABCDEF";

	if (variant_obj) {
		zend_string *variant_name = Z_STR_P(zend_enum_fetch_case_name(variant_obj));
		if (zend_string_equals_literal(variant_name, "Lower")) {
			variant_alphabet = "0123456789abcdef";
		}
	}

	bool forgiving = false;
	if (decoding_mode) {
		if (zend_string_equals_literal(Z_STR_P(zend_enum_fetch_case_name(decoding_mode)), "Forgiving")) {
			forgiving = true;
			variant_alphabet = "0123456789ABCDEF";
		}
	}

	if (is_constant_time(timing_mode, /* default */ false)) {
		zend_throw_error(zend_ce_error, "Not implemented");
		RETURN_THROWS();
	} else {
		zend_string *result = zend_string_safe_alloc(ZSTR_LEN(data) / 2, sizeof(char), 0, 0);
		size_t result_len = 0;

		bool hi = true;
		unsigned char tmp = 0;
		for (size_t i = 0; i < ZSTR_LEN(data); i++) {
			unsigned char current = ZSTR_VAL(data)[i];
			if (forgiving) {
				current = toupper(current);
			}

			char *offset;
			if (UNEXPECTED((offset = strchr(variant_alphabet, current)) == NULL)) {
				if (current == '\r' || current == '\t' || current == '\n' || current == ' ') {
					continue;
				}

				zend_string_free(result);
				zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid character", 0);
				RETURN_THROWS();
			}
			unsigned char value = offset - variant_alphabet;
			ZEND_ASSERT(value <= 0xf);

			if (hi) {
				tmp |= value << 4;
			} else {
				tmp |= value;
				ZSTR_VAL(result)[result_len++] = tmp;
				tmp = 0;
			}

			hi = !hi;
		}
		if (!hi) {
			zend_string_free(result);
			zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid length", 0);
			RETURN_THROWS();
		}
		ZSTR_LEN(result) = result_len;
		ZSTR_VAL(result)[result_len] = '\0';

		RETURN_NEW_STR(result);
	}
}
