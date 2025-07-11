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
  | Author: Wez Furlong <wez@php.net>                                    |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_PDO_SQLITE_INT_H
#define PHP_PDO_SQLITE_INT_H

#include <sqlite3.h>

typedef struct {
	const char *file;
	int line;
	unsigned int errcode;
	char *errmsg;
} pdo_sqlite_error_info;

struct pdo_sqlite_func {
	struct pdo_sqlite_func *next;

	int argc;
	zend_string *funcname;

	/* accelerated callback references */
	zend_fcall_info_cache func;
	zend_fcall_info_cache step;
	zend_fcall_info_cache fini;
};

struct pdo_sqlite_collation {
	struct pdo_sqlite_collation *next;

	zend_string *name;
	zend_fcall_info_cache callback;
};

typedef struct {
	sqlite3 *db;
	pdo_sqlite_error_info einfo;
	struct pdo_sqlite_func *funcs;
	struct pdo_sqlite_collation *collations;
	zend_fcall_info_cache authorizer_fcc;
} pdo_sqlite_db_handle;

typedef struct {
	pdo_sqlite_db_handle 	*H;
	sqlite3_stmt *stmt;
	unsigned pre_fetched:1;
	unsigned done:1;
} pdo_sqlite_stmt;

extern const pdo_driver_t pdo_sqlite_driver;

extern int pdo_sqlite_scanner(pdo_scanner_t *s);

extern int _pdo_sqlite_error(pdo_dbh_t *dbh, pdo_stmt_t *stmt, const char *file, int line);
#define pdo_sqlite_error(s) _pdo_sqlite_error(s, NULL, __FILE__, __LINE__)
#define pdo_sqlite_error_stmt(s) _pdo_sqlite_error(stmt->dbh, stmt, __FILE__, __LINE__)

extern const struct pdo_stmt_methods sqlite_stmt_methods;

enum {
	PDO_SQLITE_ATTR_OPEN_FLAGS = PDO_ATTR_DRIVER_SPECIFIC,
	PDO_SQLITE_ATTR_READONLY_STATEMENT,
	PDO_SQLITE_ATTR_EXTENDED_RESULT_CODES,
	PDO_SQLITE_ATTR_BUSY_STATEMENT,
	PDO_SQLITE_ATTR_EXPLAIN_STATEMENT
};

typedef int pdo_sqlite_create_collation_callback(void*, int, const void*, int, const void*);

void pdo_sqlite_create_function_internal(INTERNAL_FUNCTION_PARAMETERS);
void pdo_sqlite_create_aggregate_internal(INTERNAL_FUNCTION_PARAMETERS);
void pdo_sqlite_create_collation_internal(INTERNAL_FUNCTION_PARAMETERS, pdo_sqlite_create_collation_callback callback);

#endif
