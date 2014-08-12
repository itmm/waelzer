#include "t_logger.h"

#include "../2.3-strings/str.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *messages = NULL;

void t_log_adapter_fn(const char *file, int line, const char *format, va_list args) {
	static char buffer[100];
	vsnprintf(buffer, sizeof(buffer), format, args);
	buffer[sizeof(buffer) - 1] = 0;
	char *split = str_is_empty(messages) ? NULL : "\n";
	char *result = str_cons(3, messages, split, buffer);
	str_free(messages);
	messages = result;
}

char *t_log_adapter_copy_messages() {
	return str_cons(1, messages);
}

void t_log_adapter_clear_messages() {
	messages = str_free(messages);
}

