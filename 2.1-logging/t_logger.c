#include "t_logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *messages = NULL;

void t_log_adapter_fn(const char *file, int line, const char *format, va_list args) {
	static char buffer[100];
	vsnprintf(buffer, sizeof(buffer), format, args);
	buffer[sizeof(buffer) - 1] = 0;
	char *src = messages ? : "";
	char *result = malloc(strlen(src) + strlen(buffer) + 2);
	if (result) {
		strcpy(result, src);
		if (messages) { strcat(result, "\n"); }
		strcat(result, buffer);
	}
	if (messages) { free(messages); }
	messages = result;
}

char *t_log_adapter_copy_messages() {
	char *src = messages ? : "";
	char *result = malloc(strlen(src) + 1);
	if (result) { strcpy(result, src); }
	return result;
}

void t_log_adapter_clear_messages() {
	if (messages) {
		free(messages);
		messages = NULL;
	}
}

