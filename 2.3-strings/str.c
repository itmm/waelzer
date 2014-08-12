#include "str.h"

#include "../2.1-logging/log.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static char empty[1] = { 0 };

char *str_cons(int count, ...) {
	return_unless(count >= 0, empty, "count must not be negative");
	if (!count) { return empty; }

	va_list args1;
	va_list args2;
	va_start(args1, count);
	va_copy(args2, args1);

	size_t length = 0;

	for (int i = count; i; --i) {
		const char *current = va_arg(args1, const char *);
		if (current) length += strlen(current);
	}
	va_end(args1);

	char *result = malloc(length + 1);
	if (!result) {
		va_end(args2);
		return_unless(false, empty, "Can't alloc result string");
	}

	char *tail = result;
	for (int i = count; i; --i) {
		const char *current = va_arg(args2, const char *);
		size_t len = current ? strlen(current) : 0;
		memcpy(tail, current, len);
		tail += len;
	}
	*tail = 0;
	va_end(args2);

	return result;
}

bool str_is_empty(const char *str) {
	return !str || str == empty;
}

char *str_free(char *str) {
	if (!str_is_empty(str)) free(str);
	return empty;
}

char *str_empty() {
	return empty;
}

