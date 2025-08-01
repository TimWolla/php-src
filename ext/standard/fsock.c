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
   | Authors: Paul Panotzki - Bunyip Information Systems                  |
   |          Jim Winstead <jimw@php.net>                                 |
   |          Sascha Schumann <sascha@schumann.cx>                        |
   +----------------------------------------------------------------------+
*/

#include "php.h"
#include "php_globals.h"
#include <stdlib.h>
#include <stddef.h>
#include "php_network.h"
#include "file.h"

static size_t php_fsockopen_format_host_port(char **message, const char *prefix, size_t prefix_len,
	const char *host, size_t host_len, zend_long port)
{
    char portbuf[32];
    int portlen = snprintf(portbuf, sizeof(portbuf), ":" ZEND_LONG_FMT, port);
    size_t total_len = prefix_len + host_len + portlen;

    char *result = emalloc(total_len + 1); 

	if (prefix_len > 0) {
    	memcpy(result, prefix, prefix_len);
	}
    memcpy(result + prefix_len, host, host_len);
    memcpy(result + prefix_len + host_len, portbuf, portlen);

    result[total_len] = '\0';

    *message = result;

	return total_len;
}

/* {{{ php_fsockopen() */

static void php_fsockopen_stream(INTERNAL_FUNCTION_PARAMETERS, int persistent)
{
	char *host;
	size_t host_len;
	zend_long port = -1;
	zval *zerrno = NULL, *zerrstr = NULL;
	double timeout;
	bool timeout_is_null = 1;
#ifndef PHP_WIN32
	time_t conv;
#else
	long conv;
#endif
	struct timeval tv;
	char *hashkey = NULL;
	php_stream *stream = NULL;
	int err;
	char *hostname = NULL;
	size_t hostname_len;
	zend_string *errstr = NULL;

	ZEND_PARSE_PARAMETERS_START(1, 5)
		Z_PARAM_STRING(host, host_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(port)
		Z_PARAM_ZVAL(zerrno)
		Z_PARAM_ZVAL(zerrstr)
		Z_PARAM_DOUBLE_OR_NULL(timeout, timeout_is_null)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_FALSE;

	if (timeout_is_null) {
		timeout = (double)FG(default_socket_timeout);
	}

	if (persistent) {
		php_fsockopen_format_host_port(&hashkey, "pfsockopen__", strlen("pfsockopen__"), host,
				host_len, port);
	}

	if (port > 0) {
		hostname_len = php_fsockopen_format_host_port(&hostname, "", 0, host, host_len, port);
	} else {
		hostname_len = host_len;
		hostname = host;
	}

	/* prepare the timeout value for use */
	if (timeout != -1.0 && !(timeout >= 0.0 && timeout <= (double) PHP_TIMEOUT_ULL_MAX / 1000000.0)) {
		if (port > 0) {
			efree(hostname);
		}

		if (hashkey) {
			efree(hashkey);
		}

		zend_argument_value_error(6, "must be -1 or between 0 and " ZEND_ULONG_FMT, ((double) PHP_TIMEOUT_ULL_MAX / 1000000.0));
		RETURN_THROWS();
	} else {
#ifndef PHP_WIN32
		conv = (time_t) (timeout * 1000000.0);
		tv.tv_sec = conv / 1000000;
#else
		conv = (long) (timeout * 1000000.0);
		tv.tv_sec = conv / 1000000;
#endif
		tv.tv_usec = conv % 1000000;
	}

	stream = php_stream_xport_create(hostname, hostname_len, REPORT_ERRORS,
			STREAM_XPORT_CLIENT | STREAM_XPORT_CONNECT, hashkey, &tv, NULL, &errstr, &err);

	if (port > 0) {
		efree(hostname);
	}
	if (stream == NULL) {
		php_error_docref(NULL, E_WARNING, "Unable to connect to %s:" ZEND_LONG_FMT " (%s)", host, port, errstr == NULL ? "Unknown error" : ZSTR_VAL(errstr));
	}

	if (hashkey) {
		efree(hashkey);
	}

	if (stream == NULL) {
		if (zerrno) {
			ZEND_TRY_ASSIGN_REF_LONG(zerrno, err);
		}
		if (errstr) {
			if (zerrstr) {
				ZEND_TRY_ASSIGN_REF_STR(zerrstr, errstr);
			} else {
				zend_string_release(errstr);
			}
		}

		RETURN_FALSE;
	}

	if (zerrno) {
		ZEND_TRY_ASSIGN_REF_LONG(zerrno, 0);
	}
	if (zerrstr) {
		ZEND_TRY_ASSIGN_REF_EMPTY_STRING(zerrstr);
	}

	if (errstr) {
		zend_string_release_ex(errstr, 0);
	}

	php_stream_to_zval(stream, return_value);
}

/* }}} */

/* {{{ Open Internet or Unix domain socket connection */
PHP_FUNCTION(fsockopen)
{
	php_fsockopen_stream(INTERNAL_FUNCTION_PARAM_PASSTHRU, 0);
}
/* }}} */

/* {{{ Open persistent Internet or Unix domain socket connection */
PHP_FUNCTION(pfsockopen)
{
	php_fsockopen_stream(INTERNAL_FUNCTION_PARAM_PASSTHRU, 1);
}
/* }}} */
