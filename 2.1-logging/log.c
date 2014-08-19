/*>
## Implementing the Logging

First we need to include the interface and the io header.
<*/
#include "log.h"

#include <stdio.h>
/*>
### The default log adapter

The logging takes place in the default log adapter. It writes the current file and line number and the log message to `stderr`. Also a newline is written afterwards.
<*/
static void default_log_adapter(const char *file, int line, const char *format, va_list args) {
	fprintf(stderr, "%s:%d ", file, line);
	vfprintf(stderr, format, args);
	fputc('\n', stderr);
}
/*>
The current adapter is set to the default adapter
<*/
static log_adapter_fn adapter = default_log_adapter;
/*>
### Do the Logging

The function `_do_log` converts the variable arguments into a `va_list` and calls the adapter, if one is present.
<*/
void _do_log(const char *file, int line, const char *format,...) {
	if (adapter) {
		va_list args;
		va_start(args, format);
		adapter(file, line, format, args);
		va_end(args);
	}
}
/*>
### Changing the log adapter

The function `set_log_adapter` can be used to change the current adapter. The previous adapter will be returned.
<*/
log_adapter_fn set_log_adapter(log_adapter_fn log_adapter) {
	log_adapter_fn old = adapter;
	adapter = log_adapter;
	return old;
}
/*>
Logging can be disabled, by setting the adapter to `NULL`.
<*/
log_adapter_fn disable_logging() {
	return set_log_adapter(NULL);
}
/*>
The `default_log_adapter` can be set. It was either this function, or make the default adapter public. I think, this is the cleaner way to hide the details about the default function.
<*/
log_adapter_fn set_default_logging() {
	return set_log_adapter(default_log_adapter);
}
