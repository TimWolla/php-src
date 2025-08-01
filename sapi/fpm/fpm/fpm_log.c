	/* (c) 2009 Jerome Loyet */

#include "php.h"
#include "SAPI.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef HAVE_TIMES
#include <sys/times.h>
#endif

#include "fpm_config.h"
#include "fpm_log.h"
#include "fpm_clock.h"
#include "fpm_process_ctl.h"
#include "fpm_signals.h"
#include "fpm_scoreboard.h"
#include "fastcgi.h"
#include "zlog.h"

static char *fpm_log_format = NULL;
static int fpm_log_fd = -1;
static struct key_value_s *fpm_access_suppress_paths = NULL;
static struct zlog_stream fpm_log_stream;

static int fpm_access_log_suppress(struct fpm_scoreboard_proc_s *proc);

int fpm_log_open(int reopen) /* {{{ */
{
	struct fpm_worker_pool_s *wp;
	int ret = 1;

	int fd;
	for (wp = fpm_worker_all_pools; wp; wp = wp->next) {
		if (!wp->config->access_log) {
			continue;
		}
		ret = 0;

		fd = open(wp->config->access_log, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
		if (0 > fd) {
			zlog(ZLOG_SYSERROR, "failed to open access log (%s)", wp->config->access_log);
			return -1;
		} else {
			zlog(ZLOG_DEBUG, "open access log (%s)", wp->config->access_log);
		}

		if (reopen) {
			dup2(fd, wp->log_fd);
			close(fd);
			fd = wp->log_fd;
			fpm_pctl_kill_all(SIGQUIT);
		} else {
			wp->log_fd = fd;
		}

		if (0 > fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC)) {
			zlog(ZLOG_WARNING, "failed to change attribute of access_log");
		}
	}

	return ret;
}
/* }}} */

/* }}} */
int fpm_log_init_child(struct fpm_worker_pool_s *wp)  /* {{{ */
{
	if (!wp || !wp->config) {
		return -1;
	}

	if (wp->config->access_log && *wp->config->access_log) {
		if (wp->config->access_format) {
			fpm_log_format = strdup(wp->config->access_format);
		}
	}

	for (struct key_value_s *kv = wp->config->access_suppress_paths; kv; kv = kv->next) {
		struct key_value_s *kvcopy = calloc(1, sizeof(*kvcopy));
		if (kvcopy == NULL) {
			zlog(ZLOG_ERROR, "unable to allocate memory while opening the access log");
			return -1;
		}
		kvcopy->value = strdup(kv->value);
		kvcopy->next = fpm_access_suppress_paths;
		fpm_access_suppress_paths = kvcopy;
	}

	if (fpm_log_fd == -1) {
		fpm_log_fd = wp->log_fd;
	}
	zlog_stream_init_ex(&fpm_log_stream, ZLOG_ACCESS_LOG, fpm_log_fd);
	zlog_stream_set_wrapping(&fpm_log_stream, 0);

	for (wp = fpm_worker_all_pools; wp; wp = wp->next) {
		if (wp->log_fd > -1 && wp->log_fd != fpm_log_fd) {
			close(wp->log_fd);
			wp->log_fd = -1;
		}
	}

	return 0;
}
/* }}} */

int fpm_log_write(char *log_format) /* {{{ */
{
	char *s;
	bool test, token = false;
	struct fpm_scoreboard_proc_s proc, *proc_p;
	struct fpm_scoreboard_s *scoreboard;
	struct zlog_stream *stream;
	char tmp[129];
	char format[129];
	time_t now_epoch;
#ifdef HAVE_TIMES
	clock_t tms_total;
#endif

	if (!log_format && (!fpm_log_format || fpm_log_fd == -1)) {
		return -1;
	}

	if (!log_format) {
		log_format = fpm_log_format;
		test = false;
	} else {
		test = true;
	}

	now_epoch = time(NULL);

	if (!test) {
		scoreboard = fpm_scoreboard_get();
		if (!scoreboard) {
			zlog(ZLOG_WARNING, "unable to get scoreboard while preparing the access log");
			return -1;
		}
		proc_p = fpm_scoreboard_proc_acquire(NULL, -1, 0);
		if (!proc_p) {
			zlog(ZLOG_WARNING, "[pool %s] Unable to acquire shm slot while preparing the access log", scoreboard->pool);
			return -1;
		}
		proc = *proc_p;
		fpm_scoreboard_proc_release(proc_p);

		if (UNEXPECTED(fpm_access_log_suppress(&proc))) {
			return -1;
		}
	}


	s = log_format;
	stream = &fpm_log_stream;
	zlog_stream_start(stream);

	while (*s != '\0') {
		if (!token && *s == '%') {
			token = true;
			memset(format, '\0', sizeof(format)); /* reset format */
			s++;
			continue;
		}

		if (token) {
			token = false;
			switch (*s) {

				case '%': /* '%' */
					zlog_stream_char(stream, '%');
					break;

#ifdef HAVE_TIMES
				case 'C': /* %CPU */
					if (format[0] == '\0' || !strcasecmp(format, "total")) {
						if (!test) {
							tms_total = proc.last_request_cpu.tms_utime + proc.last_request_cpu.tms_stime + proc.last_request_cpu.tms_cutime + proc.last_request_cpu.tms_cstime;
						}
					} else if (!strcasecmp(format, "user")) {
						if (!test) {
							tms_total = proc.last_request_cpu.tms_utime + proc.last_request_cpu.tms_cutime;
						}
					} else if (!strcasecmp(format, "system")) {
						if (!test) {
							tms_total = proc.last_request_cpu.tms_stime + proc.last_request_cpu.tms_cstime;
						}
					} else {
						zlog(ZLOG_WARNING, "only 'total', 'user' or 'system' are allowed as a modifier for %%%c ('%s')", *s, format);
						return -1;
					}

					format[0] = '\0';
					if (!test) {
						zlog_stream_format(stream, "%.2f", tms_total / fpm_scoreboard_get_tick() / (proc.cpu_duration.tv_sec + proc.cpu_duration.tv_usec / 1000000.) * 100.);
					}
					break;
#endif

				case 'd': /* duration µs */
					/* seconds */
					if (format[0] == '\0' || !strcasecmp(format, "seconds")) {
						if (!test) {
							zlog_stream_format(stream, "%.3f", proc.duration.tv_sec + proc.duration.tv_usec / 1000000.);
						}

					/* milliseconds */
					} else if (!strcasecmp(format, "milliseconds") || !strcasecmp(format, "milli") ||
						   /* mili/miliseconds are supported for backwards compatibility */
						   !strcasecmp(format, "miliseconds") || !strcasecmp(format, "mili")
					) {
						if (!test) {
							zlog_stream_format(stream, "%.3f", proc.duration.tv_sec * 1000. + proc.duration.tv_usec / 1000.);
						}

					/* microseconds */
					} else if (!strcasecmp(format, "microseconds") || !strcasecmp(format, "micro")) {
						if (!test) {
							zlog_stream_format(stream, "%lu", (unsigned long)(proc.duration.tv_sec * 1000000UL + proc.duration.tv_usec));
						}

					} else {
						zlog(ZLOG_WARNING, "only 'seconds', 'milli', 'milliseconds', 'micro' or 'microseconds' are allowed as a modifier for %%%c ('%s')", *s, format);
						return -1;
					}
					format[0] = '\0';
					break;

				case 'e': /* fastcgi env  */
					if (format[0] == '\0') {
						zlog(ZLOG_WARNING, "the name of the environment variable must be set between embraces for %%%c", *s);
						return -1;
					}

					if (!test) {
						char *env = fcgi_getenv((fcgi_request*) SG(server_context), format, strlen(format));
						zlog_stream_cstr(stream, env ? env : "-");
					}
					format[0] = '\0';
					break;

				case 'f': /* script */
					if (!test) {
						zlog_stream_cstr(stream,  *proc.script_filename ? proc.script_filename : "-");
					}
					break;

				case 'l': /* content length */
					if (!test) {
						zlog_stream_format(stream, "%zu", proc.content_length);
					}
					break;

				case 'm': /* method */
					if (!test) {
						zlog_stream_cstr(stream, *proc.request_method ? proc.request_method : "-");
					}
					break;

				case 'M': /* memory */
					/* seconds */
					if (format[0] == '\0' || !strcasecmp(format, "bytes")) {
						if (!test) {
							zlog_stream_format(stream, "%zu", proc.memory);
						}

					/* kilobytes */
					} else if (!strcasecmp(format, "kilobytes") || !strcasecmp(format, "kilo")) {
						if (!test) {
							zlog_stream_format(stream, "%zu", proc.memory / 1024);
						}

					/* megabytes */
					} else if (!strcasecmp(format, "megabytes") || !strcasecmp(format, "mega")) {
						if (!test) {
							zlog_stream_format(stream, "%zu", proc.memory / 1024 / 1024);
						}

					} else {
						zlog(ZLOG_WARNING, "only 'bytes', 'kilo', 'kilobytes', 'mega' or 'megabytes' are allowed as a modifier for %%%c ('%s')", *s, format);
						return -1;
					}
					format[0] = '\0';
					break;

				case 'n': /* pool name */
					if (!test) {
						zlog_stream_cstr(stream, scoreboard->pool[0] ? scoreboard->pool : "-");
					}
					break;

				case 'o': /* header output  */
					if (format[0] == '\0') {
						zlog(ZLOG_WARNING, "the name of the header must be set between embraces for %%%c", *s);
						return -1;
					}
					if (!test) {
						sapi_header_struct *h;
						zend_llist_position pos;
						sapi_headers_struct *sapi_headers = &SG(sapi_headers);
						size_t format_len = strlen(format);
						ssize_t written = 0;

						h = (sapi_header_struct*)zend_llist_get_first_ex(&sapi_headers->headers, &pos);
						while (h) {
							char *header;
							if (!h->header_len) {
								h = (sapi_header_struct*)zend_llist_get_next_ex(&sapi_headers->headers, &pos);
								continue;
							}
							if (!strstr(h->header, format)) {
								h = (sapi_header_struct*)zend_llist_get_next_ex(&sapi_headers->headers, &pos);
								continue;
							}

							/* test if enough char after the header name + ': ' */
							if (h->header_len <= format_len + 2) {
								h = (sapi_header_struct*)zend_llist_get_next_ex(&sapi_headers->headers, &pos);
								continue;
							}

							if (h->header[format_len] != ':' || h->header[format_len + 1] != ' ') {
								h = (sapi_header_struct*)zend_llist_get_next_ex(&sapi_headers->headers, &pos);
								continue;
							}

							header = h->header + format_len + 2;
							written += zlog_stream_cstr(stream, header && *header ? header : "-");

							/* found, done */
							break;
						}
						if (!written) {
							zlog_stream_char(stream, '-');
						}
					}
					format[0] = '\0';
					break;

				case 'p': /* PID */
					if (!test) {
						zlog_stream_format(stream, "%ld", (long)getpid());
					}
					break;

				case 'P': /* PID */
					if (!test) {
						zlog_stream_format(stream, "%ld", (long)getppid());
					}
					break;

				case 'q': /* query_string */
					if (!test) {
						zlog_stream_cstr(stream, proc.query_string);
					}
					break;

				case 'Q': /* '?' */
					if (!test) {
						zlog_stream_cstr(stream, *proc.query_string  ? "?" : "");
					}
					break;

				case 'r': /* request URI */
					if (!test) {
						zlog_stream_cstr(stream, proc.request_uri);
					}
					break;

				case 'R': /* remote IP address */
					if (!test) {
						const char *tmp = fcgi_get_last_client_ip();
						zlog_stream_cstr(stream, tmp ? tmp : "-");
					}
					break;

				case 's': /* status */
					if (!test) {
						zlog_stream_format(stream, "%d", SG(sapi_headers).http_response_code);
					}
					break;

				case 'T':
				case 't': /* time */
					if (!test) {
						time_t *t;
						if (*s == 't') {
							t = &proc.accepted_epoch;
						} else {
							t = &now_epoch;
						}
						if (format[0] == '\0') {
							strftime(tmp, sizeof(tmp) - 1, "%d/%b/%Y:%H:%M:%S %z", localtime(t));
						} else {
							strftime(tmp, sizeof(tmp) - 1, format, localtime(t));
						}
						zlog_stream_cstr(stream, tmp);
					}
					format[0] = '\0';
					break;

				case 'u': /* remote user */
					if (!test) {
						zlog_stream_cstr(stream, proc.auth_user);
					}
					break;

				case '{': /* complex var */
					token = 1;
					{
						char *start;
						size_t l;

						start = ++s;

						while (*s != '\0') {
							if (*s == '}') {
								l = s - start;

								if (l >= sizeof(format) - 1) {
									l = sizeof(format) - 1;
								}

								memcpy(format, start, l);
								format[l] = '\0';
								break;
							}
							s++;
						}
						if (s[1] == '\0') {
							zlog(ZLOG_WARNING, "missing closing embrace in the access.format");
							return -1;
						}
					}
					break;

				default:
					zlog(ZLOG_WARNING, "Invalid token in the access.format (%%%c)", *s);
					return -1;
			}

			if (*s != '}' && format[0] != '\0') {
				zlog(ZLOG_WARNING, "embrace is not allowed for modifier %%%c", *s);
				return -1;
			}
			s++;

			if (zlog_stream_is_over_limit(stream)) {
				zlog(ZLOG_NOTICE, "the log buffer is over the configured limit. The access log request has been truncated.");
				break;
			}
			continue;
		}

		if (zlog_stream_is_over_limit(stream)) {
			zlog(ZLOG_NOTICE, "the log buffer is over the configured limit. The access log request has been truncated.");
			break;
		}

		if (!test) {
			// push the normal char to the output buffer
			zlog_stream_char(stream, *s);
		}
		s++;
	}

	if (!test) {
		zlog_stream_finish(stream);
	}

	return 0;
}
/* }}} */

static int fpm_access_log_suppress(struct fpm_scoreboard_proc_s *proc)
{
	// Never suppress when query string is passed
	if (proc->query_string[0] != '\0') {
		return 0;
	}

	// Never suppress if request method is not GET or HEAD
	if (
		strcmp(proc->request_method, "GET") != 0
		&& strcmp(proc->request_method, "HEAD") != 0
	) {
		return 0;
	}

	// Never suppress when response code does not indicate success
	if (SG(sapi_headers).http_response_code < 200 || SG(sapi_headers).http_response_code > 299) {
		return 0;
	}

	// Never suppress when a body has been sent
	if (SG(request_info).content_length > 0) {
		return 0;
	}

	// Suppress when request URI is an exact match for one of our entries
	for (struct key_value_s *kv = fpm_access_suppress_paths; kv; kv = kv->next) {
		if (kv->value && strcmp(kv->value, proc->request_uri) == 0) {
			return 1;
		}
	}

	return 0;
}