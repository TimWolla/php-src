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
#include "encoding_decl.h"

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

PHP_FUNCTION(Encoding_base16_encode)
{
	zend_string *data;
	zend_enum_Encoding_Base16 variant = ZEND_ENUM_Encoding_Base16_Upper;
	zend_enum_Encoding_TimingMode timing_mode = ZEND_ENUM_Encoding_TimingMode_Variable;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_ENUM(variant, encoding_ce_Base16);
		Z_PARAM_ENUM(timing_mode, encoding_ce_TimingMode);
	ZEND_PARSE_PARAMETERS_END();

	const char *variant_alphabet;
	switch (variant) {
	case ZEND_ENUM_Encoding_Base16_Upper:
		variant_alphabet = "0123456789ABCDEF";
		break;
	case ZEND_ENUM_Encoding_Base16_Lower:
		variant_alphabet = "0123456789abcdef";
		break;
	}

	if (timing_mode == ZEND_ENUM_Encoding_TimingMode_Constant) {
		zend_throw_error(zend_ce_error, "Not implemented");
		RETURN_THROWS();
	} else {
		zend_string *result = zend_string_safe_alloc(ZSTR_LEN(data), 2, 0, 0);

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
	zend_enum_Encoding_Base16 variant = ZEND_ENUM_Encoding_Base16_Upper;
	zend_enum_Encoding_DecodingMode decoding_mode = ZEND_ENUM_Encoding_DecodingMode_Strict;
	zend_enum_Encoding_TimingMode timing_mode = ZEND_ENUM_Encoding_TimingMode_Variable;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_ENUM(variant, encoding_ce_Base16);
		Z_PARAM_ENUM(decoding_mode, encoding_ce_DecodingMode);
		Z_PARAM_ENUM(timing_mode, encoding_ce_TimingMode);
	ZEND_PARSE_PARAMETERS_END();

	const char *variant_alphabet;
	switch (variant) {
	case ZEND_ENUM_Encoding_Base16_Upper:
		variant_alphabet = "0123456789ABCDEF";
		break;
	case ZEND_ENUM_Encoding_Base16_Lower:
		variant_alphabet = "0123456789abcdef";
		break;
	}

	bool forgiving = false;
	if (decoding_mode == ZEND_ENUM_Encoding_DecodingMode_Forgiving) {
		forgiving = true;
		variant_alphabet = "0123456789ABCDEF";
	}

	if (timing_mode == ZEND_ENUM_Encoding_TimingMode_Constant) {
		zend_throw_error(zend_ce_error, "Not implemented");
		RETURN_THROWS();
	} else {
		zend_string *result = zend_string_alloc(ZSTR_LEN(data) / 2, 0);
		size_t result_len = 0;

		uint8_t n = 0;
		unsigned char chunk[2] = {0};
		int invalid = 0;
		for (size_t i = 0; i < ZSTR_LEN(data); i++) {
			unsigned char current = ZSTR_VAL(data)[i];
			if (forgiving) {
				current = toupper(current);
			}

			char *offset = strchr(variant_alphabet, current);
			int not_whitespace = (current - '\r') * (current - '\t') * (current - '\n') * (current - ' ');
			invalid |= !offset * not_whitespace * current;

			unsigned char value = offset - variant_alphabet;

			chunk[n] = value;
			n += !!not_whitespace;

			if (n == 2) {
				ZSTR_VAL(result)[result_len++] = (chunk[0] << 4) | (chunk[1]);
				n = 0;
			}
		}
		if (invalid) {
			zend_string_free(result);
			zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid character", 0);
			RETURN_THROWS();
		}
		if (n != 0) {
			zend_string_free(result);
			zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid length", 0);
			RETURN_THROWS();
		}
		ZSTR_LEN(result) = result_len;
		ZSTR_VAL(result)[result_len] = '\0';

		RETURN_NEW_STR(result);
	}
}

PHP_FUNCTION(Encoding_base32_encode)
{
	zend_string *data;
	zend_object *variant_obj = NULL;
	zend_enum_Encoding_Base32 variant = ZEND_ENUM_Encoding_Base32_Ascii;
	zend_enum_Encoding_TimingMode timing_mode = ZEND_ENUM_Encoding_TimingMode_Variable;
	zend_enum_Encoding_PaddingMode padding_mode = ZEND_ENUM_Encoding_PaddingMode_VariantControlled;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_OBJ_OF_CLASS(variant_obj, encoding_ce_Base32);
		Z_PARAM_ENUM(padding_mode, encoding_ce_PaddingMode);
		Z_PARAM_ENUM(timing_mode, encoding_ce_TimingMode);
	ZEND_PARSE_PARAMETERS_END();

	if (variant_obj) {
		variant = zend_enum_fetch_case_id(variant_obj);
	}

	const char *variant_alphabet;
	bool padding;
	switch (variant) {
	case ZEND_ENUM_Encoding_Base32_Ascii:
		variant_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		padding = true;
		break;
	case ZEND_ENUM_Encoding_Base32_Hex:
		variant_alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
		padding = true;
		break;
	case ZEND_ENUM_Encoding_Base32_Crockford:
		variant_alphabet = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		padding = false;
		break;
	case ZEND_ENUM_Encoding_Base32_Z:
		variant_alphabet = "ybndrfg8ejkmcpqxot1uwisza345h769";
		padding = false;
		break;
	}

	switch (padding_mode) {
	case ZEND_ENUM_Encoding_PaddingMode_StripPadding:
		padding = false;
		break;
	case ZEND_ENUM_Encoding_PaddingMode_PreservePadding:
		padding = true;
		switch (variant) {
		case ZEND_ENUM_Encoding_Base32_Crockford:
		case ZEND_ENUM_Encoding_Base32_Z:
			zend_argument_value_error(3, "must not be PaddingMode::PreservePadding when argument #2 ($variant) is Base32::%S", Z_STR_P(zend_enum_fetch_case_name(variant_obj)));
			RETURN_THROWS();
		default:
			break;
		}
		break;
	case ZEND_ENUM_Encoding_PaddingMode_VariantControlled:
		break;
	}

	if (timing_mode == ZEND_ENUM_Encoding_TimingMode_Constant) {
		zend_throw_error(zend_ce_error, "Not implemented");
		RETURN_THROWS();
	} else {
		zend_string *result = zend_string_safe_alloc(ZSTR_LEN(data), 2, 0, 0);
		size_t result_len = 0;

		uint8_t n = 0;
		unsigned char chunk[5] = {0};
		for (size_t i = 0; i < ZSTR_LEN(data); i++) {
			unsigned char current = ZSTR_VAL(data)[i];
			chunk[n++] = current;
			if (n == 5) {
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[0] >> 3)                  ) & 0b11111];
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[0] << 2) | (chunk[1] >> 6)) & 0b11111];
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[1] >> 1)                  ) & 0b11111];
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[1] << 4) | (chunk[2] >> 4)) & 0b11111];
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[2] << 1) | (chunk[3] >> 7)) & 0b11111];
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[3] >> 2)                  ) & 0b11111];
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[3] << 3) | (chunk[4] >> 5)) & 0b11111];
				ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[4] >> 0)                  ) & 0b11111];

				n = 0;
				memset(chunk, 0, sizeof(chunk));
			}
		}
		if (n) {
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[0] >> 3)                  ) & 0b11111];
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[0] << 2) | (chunk[1] >> 6)) & 0b11111];
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[1] >> 1)                  ) & 0b11111];
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[1] << 4) | (chunk[2] >> 4)) & 0b11111];
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[2] << 1) | (chunk[3] >> 7)) & 0b11111];
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[3] >> 2)                  ) & 0b11111];
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[3] << 3) | (chunk[4] >> 5)) & 0b11111];
			ZSTR_VAL(result)[result_len++] = variant_alphabet[((chunk[4] >> 0)                  ) & 0b11111];

			uint8_t need;
			switch (n) {
			case 1:
				need = 2;
				break;
			case 2:
				need = 4;
				break;
			case 3:
				need = 5;
				break;
			case 4:
				need = 7;
				break;
			}
			result_len -= 8 - need;
			if (padding) {
				for (uint8_t i = 0; i < 8 - need; i++) {
					ZSTR_VAL(result)[result_len++] = '=';
				}
			}
		}
		ZSTR_LEN(result) = result_len;
		ZSTR_VAL(result)[result_len] = '\0';

		RETURN_NEW_STR(result);
	}
}

PHP_FUNCTION(Encoding_base32_decode)
{
	zend_string *data;
	zend_enum_Encoding_Base32 variant = ZEND_ENUM_Encoding_Base32_Ascii;
	zend_enum_Encoding_DecodingMode decoding_mode = ZEND_ENUM_Encoding_DecodingMode_Strict;
	zend_enum_Encoding_TimingMode timing_mode = ZEND_ENUM_Encoding_TimingMode_Variable;
	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STR(data)
		Z_PARAM_OPTIONAL
		Z_PARAM_ENUM(variant, encoding_ce_Base32);
		Z_PARAM_ENUM(decoding_mode, encoding_ce_DecodingMode);
		Z_PARAM_ENUM(timing_mode, encoding_ce_TimingMode);
	ZEND_PARSE_PARAMETERS_END();

	const char *variant_alphabet;
	bool padding;
	switch (variant) {
	case ZEND_ENUM_Encoding_Base32_Ascii:
		variant_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		padding = true;
		break;
	case ZEND_ENUM_Encoding_Base32_Hex:
		variant_alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
		padding = true;
		break;
	case ZEND_ENUM_Encoding_Base32_Crockford:
		variant_alphabet = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		padding = false;
		break;
	case ZEND_ENUM_Encoding_Base32_Z:
		variant_alphabet = "ybndrfg8ejkmcpqxot1uwisza345h769";
		padding = false;
		break;
	}

	bool forgiving = decoding_mode == ZEND_ENUM_Encoding_DecodingMode_Forgiving;

	if (timing_mode == ZEND_ENUM_Encoding_TimingMode_Constant) {
		zend_throw_error(zend_ce_error, "Not implemented");
		RETURN_THROWS();
	} else {
		zend_string *result = zend_string_alloc(ZSTR_LEN(data), 0);
		size_t result_len = 0;

		uint8_t n = 0;
		unsigned char chunk[8] = {0};
		size_t i = 0;
		unsigned char invalid = 0;
		for (; i < ZSTR_LEN(data); i++) {
			unsigned char current = ZSTR_VAL(data)[i];

			char *offset = strchr(variant_alphabet, current);
			int not_whitespace = (current - '\r') * (current - '\t') * (current - '\n') * (current - ' ');

			if (!offset && not_whitespace) {
				break;
			}

			unsigned char value = offset - variant_alphabet;

			chunk[n] = value;
			n += !!not_whitespace;

			if (n == 8) {
				ZSTR_VAL(result)[result_len++] = ((chunk[0] << 3) | (chunk[1] >> 2)) & 0xff;
				ZSTR_VAL(result)[result_len++] = ((chunk[1] << 6) | (chunk[2] << 1) | (chunk[3] >> 4)) & 0xff;
				ZSTR_VAL(result)[result_len++] = ((chunk[3] << 4) | (chunk[4] >> 1)) & 0xff;
				ZSTR_VAL(result)[result_len++] = ((chunk[4] << 7) | (chunk[5] << 2) | (chunk[6] >> 3)) & 0xff;
				ZSTR_VAL(result)[result_len++] = ((chunk[6] << 5) | (chunk[7])) & 0xff;
				n = 0;
			}
		}
		if (i < ZSTR_LEN(data)) {
			if (padding) {
				size_t padding_len = 0;
				for (; i < ZSTR_LEN(data); i++) {
					unsigned char current = ZSTR_VAL(data)[i];
					if (current == '\r' || current == '\t' || current == '\n' || current == ' ') {
						continue;
					}
					if (current != '=') {
						zend_string_free(result);
						zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid character", 0);
						RETURN_THROWS();
					}
					padding_len++;
					if (padding_len > 7) {
						zend_string_free(result);
						zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid padding", 0);
						RETURN_THROWS();
					}
				}
				if (n + padding_len != 8) {
					zend_string_free(result);
					zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid padding", 0);
					RETURN_THROWS();
				}
			} else {
				invalid |= i;
			}
		} else {
			if (n > 0 && padding) {
				if (!forgiving) {
					zend_string_free(result);
					zend_throw_exception(encoding_ce_UnableToDecodeException, "Missing padding", 0);
					RETURN_THROWS();
				}
			}
		}

		switch (n) {
		case 0:
			break;
		case 2:
			ZSTR_VAL(result)[result_len++] = ((chunk[0] << 3) | (chunk[1] >> 2)) & 0xff;
			invalid |= (chunk[1] & 0b00011);
			break;
		case 4:
			ZSTR_VAL(result)[result_len++] = ((chunk[0] << 3) | (chunk[1] >> 2)) & 0xff;
			ZSTR_VAL(result)[result_len++] = ((chunk[1] << 6) | (chunk[2] << 1) | (chunk[3] >> 4)) & 0xff;
			invalid |= (chunk[3] & 0b01111);
			break;
		case 5:
			ZSTR_VAL(result)[result_len++] = ((chunk[0] << 3) | (chunk[1] >> 2)) & 0xff;
			ZSTR_VAL(result)[result_len++] = ((chunk[1] << 6) | (chunk[2] << 1) | (chunk[3] >> 4)) & 0xff;
			ZSTR_VAL(result)[result_len++] = ((chunk[3] << 4) | (chunk[4] >> 1)) & 0xff;
			invalid |= (chunk[4] & 0b00001);
			break;
		case 7:
			ZSTR_VAL(result)[result_len++] = ((chunk[0] << 3) | (chunk[1] >> 2)) & 0xff;
			ZSTR_VAL(result)[result_len++] = ((chunk[1] << 6) | (chunk[2] << 1) | (chunk[3] >> 4)) & 0xff;
			ZSTR_VAL(result)[result_len++] = ((chunk[3] << 4) | (chunk[4] >> 1)) & 0xff;
			ZSTR_VAL(result)[result_len++] = ((chunk[4] << 7) | (chunk[5] << 2) | (chunk[6] >> 3)) & 0xff;
			invalid |= (chunk[6] & 0b00111);
			break;
		default:
			invalid |= 1;
			break;
		}

		if (invalid) {
			zend_string_free(result);
			zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid character", 0);
			RETURN_THROWS();
		}

		ZSTR_LEN(result) = result_len;
		ZSTR_VAL(result)[result_len] = '\0';

		RETURN_NEW_STR(result);
	}
}
