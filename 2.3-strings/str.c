#include "str.h"

#include "../2.1-logging/log.h"

#include <stdlib.h>
#include <string.h>

static char empty[1] = { 0 };

char *str_cons(int count, const char *strs[count]) {
	if (!count) { return empty; }

	size_t length = 0;
	const char **current;
	int i;

	for (current = strs, i = count; i; --i, ++current) {
		if (*current) length += strlen(*current);
	}

	char *result = malloc(length + 1);
	return_unless(result, empty, "Can't alloc result string");

	char *tail = result;
	for (current = strs, i = count; i; --i, ++current) {
		size_t len = *current ? strlen(*current) : 0;
		memcpy(tail, *current, len);
		tail += len;
	}
	*tail = 0;

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

