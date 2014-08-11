#include "log.h"

#include <stdio.h>
#include <stdlib.h>

static void default_log_adapter(const char *file, int line, const char *format, va_list args) {
	fprintf(stderr, "%s:%d ", file, line);
	vfprintf(stderr, format, args);
	fputc('\n', stderr);
}

static log_adapter_fn adapter = default_log_adapter;

void _do_log(const char *file, int line, const char *format,...) {
	if (adapter) {
		va_list args;
		va_start(args, format);
		adapter(file, line, format, args);
		va_end(args);
	}
}

log_adapter_fn set_log_adapter(log_adapter_fn log_adapter) {
	log_adapter_fn old = adapter;
	adapter = log_adapter;
	return old;
}

log_adapter_fn disable_logging() {
	return set_log_adapter(NULL);
}

log_adapter_fn set_default_logging() {
	return set_log_adapter(default_log_adapter);
}
