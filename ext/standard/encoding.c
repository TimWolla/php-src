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

/*
 * Some macros for constant-time comparisons. These work over values in
 * the 0..255 range. Returned value is 0x00 on "false", 0xFF on "true".
 *
 * Taken from https://github.com/jedisct1/libsodium/blob/cb20f6d9cd5221ad988ed45a700685c3791757f0/src/libsodium/sodium/codecs.c#L104-L115
 * Original code by Thomas Pornin.
 */
#define EQ(x, y) \
    ((((0U - ((unsigned int) (x) ^ (unsigned int) (y))) >> 8) & 0xFF) ^ 0xFF)
#define GT(x, y) ((((unsigned int) (y) - (unsigned int) (x)) >> 8) & 0xFF)
#define GE(x, y) (GT(y, x) ^ 0xFF)
#define LT(x, y) GT(y, x)
#define LE(x, y) GE(y, x)

ZEND_ATTRIBUTE_CONST static unsigned int encode_hex_digit_ct(unsigned int v, unsigned char c)
{
	return (LT(v, 10) & (v + '0'))
		| (GE(v, 10) & (v + c - 10));
}

ZEND_ATTRIBUTE_CONST static unsigned int is_ws_ct(unsigned int v)
{
	return EQ(v, '\r') | EQ(v, '\t') | EQ(v, '\n') | EQ(v, ' ');
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

	unsigned char variant_alphabet;
	switch (variant) {
	case ZEND_ENUM_Encoding_Base16_Upper:
		variant_alphabet = 'A';
		break;
	case ZEND_ENUM_Encoding_Base16_Lower:
		variant_alphabet = 'a';
		break;
	default: ZEND_UNREACHABLE();
	}

	zend_string *result = zend_string_safe_alloc(ZSTR_LEN(data), 2, 0, false);
	char *out = ZSTR_VAL(result);

	switch (timing_mode) {
	case ZEND_ENUM_Encoding_TimingMode_Constant:
		zend_throw_error(zend_ce_error, "Not implemented");
		goto fail;
	case ZEND_ENUM_Encoding_TimingMode_Variable:
		for (size_t i = 0; i < ZSTR_LEN(data); i++) {
			unsigned int c = (unsigned char)ZSTR_VAL(data)[i];

			*out++ = encode_hex_digit_ct(c >> 4, variant_alphabet);
			*out++ = encode_hex_digit_ct(c & 0xf, variant_alphabet);
		}
		break;
	default: ZEND_UNREACHABLE();
	}

	*out = '\0';
	RETURN_NEW_STR(zend_string_truncate(result, out - ZSTR_VAL(result), false));

 fail:
	zend_string_free(result);
	RETURN_THROWS();
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

	unsigned char variant_alphabet;
	switch (variant) {
	case ZEND_ENUM_Encoding_Base16_Upper:
		variant_alphabet = 'A';
		break;
	case ZEND_ENUM_Encoding_Base16_Lower:
		variant_alphabet = 'a';
		break;
	default: ZEND_UNREACHABLE();
	}

	bool forgiving = false;
	if (decoding_mode == ZEND_ENUM_Encoding_DecodingMode_Forgiving) {
		forgiving = true;
	}

	zend_string *result = zend_string_alloc(ZSTR_LEN(data) / 2, false);
	char *out = ZSTR_VAL(result);

	switch (timing_mode) {
	case ZEND_ENUM_Encoding_TimingMode_Constant:
		zend_throw_error(zend_ce_error, "Not implemented");
		goto fail;
	case ZEND_ENUM_Encoding_TimingMode_Variable: {
		uint8_t bits = 0;
		unsigned int carry = 0;
		unsigned int invalid = 0;
		for (size_t i = 0; i < ZSTR_LEN(data); i++) {
			unsigned int c = (unsigned char)ZSTR_VAL(data)[i];
			unsigned int value = (GE(c, '0') & LE(c, '9') & (c - '0'))
				| (GE(c, variant_alphabet) & LE(c, variant_alphabet + 5) & (c - variant_alphabet + 10));
			if (forgiving) {
				value |= (GE(c, variant_alphabet ^ 0x20) & LE(c, (variant_alphabet ^ 0x20) + 5) & (c - (variant_alphabet ^ 0x20) + 10));
			}
			value |= is_ws_ct(c);

			invalid |= (EQ(value, 0) & (EQ(c, '0') ^ 0xff));

			unsigned int shift = (is_ws_ct(c) ^ 0xff) & 4;
			carry = (carry << shift) | (value & (is_ws_ct(c) ^ 0xff));
			bits += shift;

			if (bits >= 8) {
				*out++ = (carry >> (bits - 8));
				bits -= 8;
			}
		}
		if (invalid) {
			zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid character", 0);
			goto fail;
		}
		if (bits > 0) {
			zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid length", 0);
			goto fail;
		}
	} break;
	default: ZEND_UNREACHABLE();
	}

	*out = '\0';
	RETURN_NEW_STR(zend_string_truncate(result, out - ZSTR_VAL(result), false));

 fail:
	zend_string_free(result);
	RETURN_THROWS();
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
	default: ZEND_UNREACHABLE();
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
	default: ZEND_UNREACHABLE();
	}

	zend_string *result = zend_string_safe_alloc(ZSTR_LEN(data), 2, 8, false);
	char *out = ZSTR_VAL(result);

	switch (timing_mode) {
	case ZEND_ENUM_Encoding_TimingMode_Constant:
		zend_throw_error(zend_ce_error, "Not implemented");
		goto fail;
	case ZEND_ENUM_Encoding_TimingMode_Variable: {
		uint8_t n = 0;
		unsigned char chunk[5] = {0};
		for (size_t i = 0; i < ZSTR_LEN(data); i++) {
			unsigned char c = ZSTR_VAL(data)[i];
			chunk[n++] = c;
			if (n == 5) {
				*out++ = variant_alphabet[((chunk[0] >> 3)                  ) & 0b11111];
				*out++ = variant_alphabet[((chunk[0] << 2) | (chunk[1] >> 6)) & 0b11111];
				*out++ = variant_alphabet[((chunk[1] >> 1)                  ) & 0b11111];
				*out++ = variant_alphabet[((chunk[1] << 4) | (chunk[2] >> 4)) & 0b11111];
				*out++ = variant_alphabet[((chunk[2] << 1) | (chunk[3] >> 7)) & 0b11111];
				*out++ = variant_alphabet[((chunk[3] >> 2)                  ) & 0b11111];
				*out++ = variant_alphabet[((chunk[3] << 3) | (chunk[4] >> 5)) & 0b11111];
				*out++ = variant_alphabet[((chunk[4] >> 0)                  ) & 0b11111];

				n = 0;
				memset(chunk, 0, sizeof(chunk));
			}
		}
		if (n > 0) {
			*out++ = variant_alphabet[((chunk[0] >> 3)                  ) & 0b11111];
			*out++ = variant_alphabet[((chunk[0] << 2) | (chunk[1] >> 6)) & 0b11111];
		}
		if (n > 1) {
			*out++ = variant_alphabet[((chunk[1] >> 1)                  ) & 0b11111];
			*out++ = variant_alphabet[((chunk[1] << 4) | (chunk[2] >> 4)) & 0b11111];
		}
		if (n > 2) {
			*out++ = variant_alphabet[((chunk[2] << 1) | (chunk[3] >> 7)) & 0b11111];
		}
		if (n > 3) {
			*out++ = variant_alphabet[((chunk[3] >> 2)                  ) & 0b11111];
			*out++ = variant_alphabet[((chunk[3] << 3) | (chunk[4] >> 5)) & 0b11111];
		}
		ZEND_ASSERT(!(n > 4));

		if (padding) {
			uint8_t padding_length = (8 - ((out - ZSTR_VAL(result)) % 8)) % 8;
			for (uint8_t i = 0; i < padding_length; i++) {
				*out++ = '=';
			}
		}

		break;
	}
	default: ZEND_UNREACHABLE();
	}

	*out = '\0';
	RETURN_NEW_STR(zend_string_truncate(result, out - ZSTR_VAL(result), false));

 fail:
	zend_string_free(result);
	RETURN_THROWS();
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
	default: ZEND_UNREACHABLE();
	}

	bool forgiving = decoding_mode == ZEND_ENUM_Encoding_DecodingMode_Forgiving;

	zend_string *result = zend_string_alloc(ZSTR_LEN(data), false);
	char *out = ZSTR_VAL(result);

	switch (timing_mode) {
	case ZEND_ENUM_Encoding_TimingMode_Constant:
		zend_throw_error(zend_ce_error, "Not implemented");
		goto fail;
	case ZEND_ENUM_Encoding_TimingMode_Variable: {
		uint8_t bits = 0;
		unsigned int carry = 0;
		size_t i = 0;
		unsigned char invalid = 0;
		for (; i < ZSTR_LEN(data); i++) {
			unsigned char c = ZSTR_VAL(data)[i];

			const char *offset = strchr(variant_alphabet, c);

			if (!offset && !is_ws_ct(c)) {
				break;
			}

			unsigned char value = (offset - variant_alphabet) & (is_ws_ct(c) ^ 0xff);

			unsigned int shift = (is_ws_ct(c) ^ 0xff) & 5;
			carry = (carry << shift) | value;
			bits += shift;
			if (bits >= 8) {
				*out++ = (carry >> (bits - 8));
				bits -= 8;
			}
		}
		if (i < ZSTR_LEN(data)) {
			if (padding) {
				size_t padding_len = 0;
				for (; i < ZSTR_LEN(data); i++) {
					unsigned int c = (unsigned char)ZSTR_VAL(data)[i];
					invalid |= (EQ(c, '=') | is_ws_ct(c)) ^ 0xff;
					padding_len += EQ(c, '=') & 1;
				}
				if (invalid) {
					zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid character", 0);
					goto fail;
				}
				if (padding_len > 7) {
					zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid padding", 0);
					goto fail;
				}
				if ((bits + (padding_len * 5)) % 8 != 0) {
					zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid padding", 0);
					goto fail;
				}
			} else {
				zend_throw_exception(encoding_ce_UnableToDecodeException, "Invalid character", 0);
				goto fail;
			}
		} else {
			if (bits > 0 && padding) {
				if (!forgiving) {
					zend_string_free(result);
					zend_throw_exception(encoding_ce_UnableToDecodeException, "Missing padding", 0);
					RETURN_THROWS();
				}
			}
		}
	} break;
	default: ZEND_UNREACHABLE();
	}

	*out = '\0';
	RETURN_NEW_STR(zend_string_truncate(result, out - ZSTR_VAL(result), false));

 fail:
	zend_string_free(result);
	RETURN_THROWS();
}
