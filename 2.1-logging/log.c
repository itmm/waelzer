#include "log.h"

#include <stdio.h>
#include <stdlib.h>

static void default_logger(const char *file, int line, const char *format, va_list args) {
	fprintf(stderr, "%s:%d ", file, line);
	vfprintf(stderr, format, args);
	fputc('\n', stderr);
}

static log_fn current = default_logger;

void _do_log(const char *file, int line, const char *format,...) {
	if (current) {
		va_list args;
		va_start(args, format);
		current(file, line, format, args);
		va_end(args);
	}
}

log_fn set_logger(log_fn logger) {
	log_fn old = current;
	current = logger;
	return old;
}

