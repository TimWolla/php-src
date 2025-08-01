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

#include <unicode/locid.h>
#include <unicode/timezone.h>
#include <unicode/ustring.h>
#include <unicode/calendar.h>
#include "intl_convertcpp.h"

#include "../common/common_date.h"

extern "C" {
#include "../php_intl.h"
#define USE_TIMEZONE_POINTER 1
#include "timezone_class.h"
#include "intl_convert.h"
#include <zend_exceptions.h>
#include <ext/date/php_date.h>
}
#include "common/common_enum.h"

using icu::Locale;
using icu::Calendar;

U_CFUNC PHP_METHOD(IntlTimeZone, __construct)
{
	zend_throw_exception( NULL,
		"An object of this type cannot be created with the new operator",
		0 );
}

U_CFUNC PHP_FUNCTION(intltz_create_time_zone)
{
	char	*str_id;
	size_t	 str_id_len;
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(str_id, str_id_len)
	ZEND_PARSE_PARAMETERS_END();

	UErrorCode status = UErrorCode();
	UnicodeString id = UnicodeString();
	if (intl_stringFromChar(id, str_id, str_id_len, &status) == FAILURE) {
		intl_error_set(NULL, status,
			"could not convert time zone id to UTF-16");
		RETURN_NULL();
	}

	//guaranteed non-null; GMT if timezone cannot be understood
	TimeZone *tz = TimeZone::createTimeZone(id);
	timezone_object_construct(tz, return_value, 1);
}

U_CFUNC PHP_FUNCTION(intltz_from_date_time_zone)
{
	zval				*zv_timezone;
	TimeZone			*tz;
	php_timezone_obj	*tzobj;
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(zv_timezone, php_date_get_timezone_ce())
	ZEND_PARSE_PARAMETERS_END();

	tzobj = Z_PHPTIMEZONE_P(zv_timezone);
	if (!tzobj->initialized) {
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"DateTimeZone object is unconstructed");
		RETURN_NULL();
	}

	tz = timezone_convert_datetimezone(tzobj->type, tzobj, false, NULL);
	if (tz == NULL) {
		RETURN_NULL();
	}

	timezone_object_construct(tz, return_value, 1);
}

U_CFUNC PHP_FUNCTION(intltz_create_default)
{
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_NONE();

	TimeZone *tz = TimeZone::createDefault();
	timezone_object_construct(tz, return_value, 1);
}

U_CFUNC PHP_FUNCTION(intltz_get_gmt)
{
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_NONE();

	timezone_object_construct(TimeZone::getGMT(), return_value, 0);
}

U_CFUNC PHP_FUNCTION(intltz_get_unknown)
{
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_NONE();

	timezone_object_construct(&TimeZone::getUnknown(), return_value, 0);
}

U_CFUNC PHP_FUNCTION(intltz_create_enumeration)
{
	zval				*arg = NULL;
	StringEnumeration	*se	  = NULL;
	intl_error_reset(NULL);

	/* double indirection to have the zend engine destroy the new zval that
	 * results from separation */
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(arg)
	ZEND_PARSE_PARAMETERS_END();

	if (arg == NULL || Z_TYPE_P(arg) == IS_NULL) {
		se = TimeZone::createEnumeration();
	} else if (Z_TYPE_P(arg) == IS_LONG) {
int_offset:
		if (UNEXPECTED(Z_LVAL_P(arg) < (zend_long)INT32_MIN ||
				Z_LVAL_P(arg) > (zend_long)INT32_MAX)) {
			intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
				"value is out of range");
			RETURN_FALSE;
		} else {
			se = TimeZone::createEnumeration((int32_t) Z_LVAL_P(arg));
		}
	} else if (Z_TYPE_P(arg) == IS_DOUBLE) {
double_offset:
		convert_to_long(arg);
		goto int_offset;
	} else if (Z_TYPE_P(arg) == IS_OBJECT || Z_TYPE_P(arg) == IS_STRING) {
		zend_long lval;
		double dval;
		if (!try_convert_to_string(arg)) {
			RETURN_THROWS();
		}
		switch (is_numeric_string(Z_STRVAL_P(arg), Z_STRLEN_P(arg), &lval, &dval, 0)) {
		case IS_DOUBLE:
			zval_ptr_dtor(arg);
			ZVAL_DOUBLE(arg, dval);
			goto double_offset;
		case IS_LONG:
			zval_ptr_dtor(arg);
			ZVAL_LONG(arg, lval);
			goto int_offset;
		}
		/* else call string version */
		se = TimeZone::createEnumeration(Z_STRVAL_P(arg));
	} else {
		// TODO Should be a TypeError
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"invalid argument type");
		RETURN_FALSE;
	}

	if (se) {
		IntlIterator_from_StringEnumeration(se, return_value);
	} else {
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"error obtaining enumeration");
		RETVAL_FALSE;
	}
}

U_CFUNC PHP_FUNCTION(intltz_count_equivalent_ids)
{
	char	*str_id;
	size_t	 str_id_len;
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(str_id, str_id_len)
	ZEND_PARSE_PARAMETERS_END();

	UErrorCode status = UErrorCode();
	UnicodeString id = UnicodeString();
	if (intl_stringFromChar(id, str_id, str_id_len, &status) == FAILURE) {
		intl_error_set(NULL, status,
			"could not convert time zone id to UTF-16");
		RETURN_FALSE;
	}

	int32_t result = TimeZone::countEquivalentIDs(id);
	RETURN_LONG((zend_long)result);
}

U_CFUNC PHP_FUNCTION(intltz_create_time_zone_id_enumeration)
{
	zend_long zoneType,
			  offset_arg;
	char	 *region		= NULL;
	size_t    region_len = 0;
	int32_t	  offset,
			 *offsetp	= NULL;
	bool arg3isnull = 1;

	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_LONG(zoneType)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING_OR_NULL(region, region_len)
		Z_PARAM_LONG_OR_NULL(offset_arg, arg3isnull)
	ZEND_PARSE_PARAMETERS_END();

	if (zoneType != UCAL_ZONE_TYPE_ANY && zoneType != UCAL_ZONE_TYPE_CANONICAL
			&& zoneType != UCAL_ZONE_TYPE_CANONICAL_LOCATION) {
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR, "bad zone type");
		RETURN_FALSE;
	}

	if (!arg3isnull) {
		if (UNEXPECTED(offset_arg < (zend_long)INT32_MIN || offset_arg > (zend_long)INT32_MAX)) {
			intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
				"offset out of bounds");
			RETURN_FALSE;
		}
		offset = (int32_t)offset_arg;
		offsetp = &offset;
	} //else leave offsetp NULL

	StringEnumeration *se;
	UErrorCode uec = UErrorCode();
	se = TimeZone::createTimeZoneIDEnumeration((USystemTimeZoneType)zoneType,
		region, offsetp, uec);
	INTL_CHECK_STATUS(uec, "error obtaining time zone id enumeration")

	IntlIterator_from_StringEnumeration(se, return_value);
}

U_CFUNC PHP_FUNCTION(intltz_get_canonical_id)
{
	char	*str_id;
	size_t	 str_id_len;
	zval	*is_systemid = NULL;
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STRING(str_id, str_id_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(is_systemid)
	ZEND_PARSE_PARAMETERS_END();

	UErrorCode status = UErrorCode();
	UnicodeString id;
	if (intl_stringFromChar(id, str_id, str_id_len, &status) == FAILURE) {
		intl_error_set(NULL, status,
			"could not convert time zone id to UTF-16");
		RETURN_FALSE;
	}

	UnicodeString result;
	UBool isSystemID;
	TimeZone::getCanonicalID(id, result, isSystemID, status);
	INTL_CHECK_STATUS(status, "error obtaining canonical ID");

	zend_string *u8str =intl_convert_utf16_to_utf8(result.getBuffer(), result.length(), &status);
	INTL_CHECK_STATUS(status,
		"could not convert time zone id to UTF-16");
	RETVAL_NEW_STR(u8str);

	if (is_systemid) { /* by-ref argument passed */
		ZEND_TRY_ASSIGN_REF_BOOL(is_systemid, isSystemID);
	}
}

U_CFUNC PHP_FUNCTION(intltz_get_region)
{
	char	*str_id;
	size_t	 str_id_len;
	char	 outbuf[3];
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(str_id, str_id_len)
	ZEND_PARSE_PARAMETERS_END();

	UErrorCode status = UErrorCode();
	UnicodeString id;
	if (intl_stringFromChar(id, str_id, str_id_len, &status) == FAILURE) {
		intl_error_set(NULL, status,
			"could not convert time zone id to UTF-16");
		RETURN_FALSE;
	}

	int32_t region_len = TimeZone::getRegion(id, outbuf, sizeof(outbuf), status);
	INTL_CHECK_STATUS(status, "error obtaining region");

	RETURN_STRINGL(outbuf, region_len);
}

U_CFUNC PHP_FUNCTION(intltz_get_tz_data_version)
{
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_NONE();

	UErrorCode status = UErrorCode();
	const char *res = TimeZone::getTZDataVersion(status);
	INTL_CHECK_STATUS(status, "error obtaining time zone data version");

	RETURN_STRING(res);
}

U_CFUNC PHP_FUNCTION(intltz_get_equivalent_id)
{
	char	   *str_id;
	size_t		str_id_len;
	zend_long	index;
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STRING(str_id, str_id_len)
		Z_PARAM_LONG(index)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(index < (zend_long)INT32_MIN || index > (zend_long)INT32_MAX)) {
		RETURN_FALSE;
	}

	UErrorCode status = UErrorCode();
	UnicodeString id;
	if (intl_stringFromChar(id, str_id, str_id_len, &status) == FAILURE) {
		intl_error_set(NULL, status,
			"could not convert time zone id to UTF-16");
		RETURN_FALSE;
	}

	const UnicodeString result = TimeZone::getEquivalentID(id, (int32_t)index);
	zend_string *u8str;

	u8str = intl_convert_utf16_to_utf8(result.getBuffer(), result.length(), &status);
	INTL_CHECK_STATUS(status, "could not convert resulting time zone id to UTF-16");
	RETVAL_NEW_STR(u8str);
}

#if U_ICU_VERSION_MAJOR_NUM >= 74
U_CFUNC PHP_FUNCTION(intltz_get_iana_id)
{
	char	*str_id;
	size_t	 str_id_len;
	intl_error_reset(NULL);

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STRING(str_id, str_id_len)
	ZEND_PARSE_PARAMETERS_END();

	UErrorCode status = UErrorCode();
	UnicodeString id;
	if (intl_stringFromChar(id, str_id, str_id_len, &status) == FAILURE) {
		intl_error_set(NULL, status,
			"could not convert time zone id to UTF-16");
		RETURN_FALSE;
	}

	UnicodeString result;
	TimeZone::getIanaID(id, result, status);
	INTL_CHECK_STATUS(status, "error obtaining IANA ID");

	zend_string *u8str = intl_convert_utf16_to_utf8(result.getBuffer(), result.length(), &status);
	INTL_CHECK_STATUS(status,
		"could not convert time zone id to UTF-16");
	RETVAL_NEW_STR(u8str);
}
#endif

U_CFUNC PHP_FUNCTION(intltz_get_id)
{
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "O",
			&object, TimeZone_ce_ptr) == FAILURE) {
		RETURN_THROWS();
	}

	TIMEZONE_METHOD_FETCH_OBJECT;

	UnicodeString id_us;
	to->utimezone->getID(id_us);

	zend_string *u8str;

	u8str = intl_convert_utf16_to_utf8(
		id_us.getBuffer(), id_us.length(), TIMEZONE_ERROR_CODE_P(to));
	INTL_METHOD_CHECK_STATUS(to, "could not convert id to UTF-8");

	RETVAL_NEW_STR(u8str);
}

U_CFUNC PHP_FUNCTION(intltz_use_daylight_time)
{
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "O",
			&object, TimeZone_ce_ptr) == FAILURE) {
		RETURN_THROWS();
	}

	TIMEZONE_METHOD_FETCH_OBJECT;

	RETURN_BOOL(to->utimezone->useDaylightTime());
}

U_CFUNC PHP_FUNCTION(intltz_get_offset)
{
	double		date;
	bool	local;
	zval		*rawOffsetArg,
				*dstOffsetArg;
	int32_t		rawOffset,
				dstOffset;
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(),
			"Odbzz", &object, TimeZone_ce_ptr, &date, &local, &rawOffsetArg,
			&dstOffsetArg) == FAILURE) {
		RETURN_THROWS();
	}

	TIMEZONE_METHOD_FETCH_OBJECT;

	to->utimezone->getOffset((UDate) date, (UBool) local, rawOffset, dstOffset,
		TIMEZONE_ERROR_CODE(to));

	INTL_METHOD_CHECK_STATUS(to, "error obtaining offset");

	ZEND_TRY_ASSIGN_REF_LONG(rawOffsetArg, rawOffset);
	ZEND_TRY_ASSIGN_REF_LONG(dstOffsetArg, dstOffset);

	RETURN_TRUE;
}

U_CFUNC PHP_FUNCTION(intltz_get_raw_offset)
{
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(),
			"O", &object, TimeZone_ce_ptr) == FAILURE) {
		RETURN_THROWS();
	}

	TIMEZONE_METHOD_FETCH_OBJECT;

	RETURN_LONG(to->utimezone->getRawOffset());
}

U_CFUNC PHP_FUNCTION(intltz_has_same_rules)
{
	zval			*other_object;
	TimeZone_object	*other_to;
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(),
			"OO", &object, TimeZone_ce_ptr, &other_object, TimeZone_ce_ptr)
			== FAILURE) {
		RETURN_THROWS();
	}
	TIMEZONE_METHOD_FETCH_OBJECT;
	other_to = Z_INTL_TIMEZONE_P(other_object);
	if (other_to->utimezone == NULL) {
		intl_errors_set(&to->err, U_ILLEGAL_ARGUMENT_ERROR, "The second IntlTimeZone is unconstructed");
		RETURN_FALSE;
	}

	RETURN_BOOL(to->utimezone->hasSameRules(*other_to->utimezone));
}

static constexpr TimeZone::EDisplayType display_types[] = {
	TimeZone::SHORT,				TimeZone::LONG,
	TimeZone::SHORT_GENERIC,		TimeZone::LONG_GENERIC,
	TimeZone::SHORT_GMT,			TimeZone::LONG_GMT,
	TimeZone::SHORT_COMMONLY_USED,	TimeZone::GENERIC_LOCATION
};

U_CFUNC PHP_FUNCTION(intltz_get_display_name)
{
	bool	daylight		= 0;
	zend_long	display_type	= TimeZone::LONG;
	const char *locale_str		= NULL;
	size_t		dummy			= 0;
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(),
			"O|bls!", &object, TimeZone_ce_ptr, &daylight, &display_type,
			&locale_str, &dummy) == FAILURE) {
		RETURN_THROWS();
	}

	bool found = false;
	for (int i = 0; !found && i < sizeof(display_types)/sizeof(*display_types); i++) {
		if (display_types[i] == display_type)
			found = true;
	}
	if (!found) {
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR, "wrong display type");
		RETURN_FALSE;
	}

	if (!locale_str) {
		locale_str = intl_locale_get_default();
	}

	TIMEZONE_METHOD_FETCH_OBJECT;

	UnicodeString result;
	to->utimezone->getDisplayName((UBool)daylight, (TimeZone::EDisplayType)display_type,
		Locale::createFromName(locale_str), result);

	zend_string *u8str = intl_convert_utf16_to_utf8(result.getBuffer(), result.length(), TIMEZONE_ERROR_CODE_P(to));
	INTL_METHOD_CHECK_STATUS(to, "could not convert resulting time zone id to UTF-16");

	RETVAL_NEW_STR(u8str);
}

U_CFUNC PHP_FUNCTION(intltz_get_dst_savings)
{
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(),
			"O", &object, TimeZone_ce_ptr) == FAILURE) {
		RETURN_THROWS();
	}

	TIMEZONE_METHOD_FETCH_OBJECT;

	RETURN_LONG((zend_long)to->utimezone->getDSTSavings());
}

U_CFUNC PHP_FUNCTION(intltz_to_date_time_zone)
{
	zval tmp;
	TIMEZONE_METHOD_INIT_VARS;

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(),
			"O", &object, TimeZone_ce_ptr) == FAILURE) {
		RETURN_THROWS();
	}

	TIMEZONE_METHOD_FETCH_OBJECT;

	zval *ret = timezone_convert_to_datetimezone(to->utimezone,
		&TIMEZONE_ERROR(to), &tmp);

	if (ret) {
		ZVAL_COPY_VALUE(return_value, ret);
	} else {
		RETURN_FALSE;
	}
}

U_CFUNC PHP_FUNCTION(intltz_get_error_code)
{
	TIMEZONE_METHOD_INIT_VARS

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "O",
			&object, TimeZone_ce_ptr) == FAILURE) {
		RETURN_THROWS();
	}

	/* Fetch the object (without resetting its last error code ). */
	to = Z_INTL_TIMEZONE_P(object);
	if (to == NULL)
		RETURN_FALSE;

	RETURN_LONG((zend_long)TIMEZONE_ERROR_CODE(to));
}

U_CFUNC PHP_FUNCTION(intltz_get_error_message)
{
	zend_string* message = NULL;
	TIMEZONE_METHOD_INIT_VARS

	if (zend_parse_method_parameters(ZEND_NUM_ARGS(), getThis(), "O",
			&object, TimeZone_ce_ptr) == FAILURE) {
		RETURN_THROWS();
	}


	/* Fetch the object (without resetting its last error code ). */
	to = Z_INTL_TIMEZONE_P(object);
	if (to == NULL)
		RETURN_FALSE;

	/* Return last error message. */
	message = intl_error_get_message(TIMEZONE_ERROR_P(to));
	RETURN_STR(message);
}

/* {{{ Translate a system timezone (e.g. "America/Los_Angeles" into a
Windows Timezone (e.g. "Pacific Standard Time")
 */
U_CFUNC PHP_FUNCTION(intltz_get_windows_id)
{
	zend_string *id, *winID;
	UnicodeString uID, uWinID;
	UErrorCode error;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(id)
	ZEND_PARSE_PARAMETERS_END();

	error = U_ZERO_ERROR;
	if (intl_stringFromChar(uID, id->val, id->len, &error) == FAILURE) {
		intl_error_set(NULL, error,
		               "could not convert time zone id to UTF-16");
		RETURN_FALSE;
	}

	error = U_ZERO_ERROR;
	TimeZone::getWindowsID(uID, uWinID, error);
	INTL_CHECK_STATUS(error, "Unable to get timezone from windows ID");
	if (uWinID.length() == 0) {
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"unknown system timezone");
		RETURN_FALSE;
	}

	error = U_ZERO_ERROR;
	winID = intl_convert_utf16_to_utf8(uWinID.getBuffer(), uWinID.length(), &error);
	INTL_CHECK_STATUS(error, "could not convert time zone id to UTF-8");
	RETURN_NEW_STR(winID);
}
/* }}} */

/* {{{ Translate a windows timezone (e.g. "Pacific Time Zone" into a
System Timezone (e.g. "America/Los_Angeles")
 */
U_CFUNC PHP_FUNCTION(intltz_get_id_for_windows_id)
{
	zend_string *winID, *region = NULL, *id;
	UnicodeString uWinID, uID;
	UErrorCode error;

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(winID)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(region)
	ZEND_PARSE_PARAMETERS_END();

	error = U_ZERO_ERROR;
	if (intl_stringFromChar(uWinID, winID->val, winID->len, &error) == FAILURE) {
		intl_error_set(NULL, error,
		               "could not convert time zone id to UTF-16");
		RETURN_FALSE;
	}

	error = U_ZERO_ERROR;
	TimeZone::getIDForWindowsID(uWinID, region ? region->val : NULL, uID, error);
	INTL_CHECK_STATUS(error, "unable to get windows ID for timezone");
	if (uID.length() == 0) {
		intl_error_set(NULL, U_ILLEGAL_ARGUMENT_ERROR,
			"unknown windows timezone");
		RETURN_FALSE;
	}

	error = U_ZERO_ERROR;
	id = intl_convert_utf16_to_utf8(uID.getBuffer(), uID.length(), &error);
	INTL_CHECK_STATUS(error, "could not convert time zone id to UTF-8");
	RETURN_NEW_STR(id);
}
/* }}} */
