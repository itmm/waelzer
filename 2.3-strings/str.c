/*>
## The implementation

First we need a couple of headers.
<*/
#include "str.h"

#include "../2.1-logging/log.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
/*>
The special empty string contains just one byte: the null terminator. We don't treat any empty string as special. It must have the same address as the special empty string to qualify for special treatment.
<*/
static char empty[1] = { 0 };
/*>
### Concatenating Strings

To concatenate the strings, we build two `va_list` of the variable arguments.  The first will be used to calculate the length of the resulting string. The second will be used in the copy phase.
<*/
char *str_cons(int count, ...) {
	return_unless(count >= 0, empty, "count must not be negative");
	if (!count) { return empty; }

	va_list args1;
	va_list args2;
	va_start(args1, count);
	va_copy(args2, args1);
/*>
The calculation of the length is straight forward. We only have to watch out for `NULL` strings, that will be silently ignored.
<*/
	size_t length = 0;
	for (int i = count; i; --i) {
		const char *current = va_arg(args1, const char *);
		if (current) length += strlen(current);
	}
	va_end(args1);
/*>
The result buffer must be one byte bigger to store the terminating null byte.
<*/
	char *result = malloc(length + 1);
	if (!result) {
		va_end(args2);
		return_unless(false, empty, "Can't alloc result string");
	}
/*>
In the copy phase, we keep a pointer to the current end of the string and advance it after each copy.
<*/
	char *tail = result;
	for (int i = count; i; --i) {
		const char *current = va_arg(args2, const char *);
		size_t len = current ? strlen(current) : 0;
		memcpy(tail, current, len);
		tail += len;
	}
/*>
Don't forget to add the terminating null byte.
<*/
	*tail = 0;
	va_end(args2);

	return result;
}
/*>
### Special empty string

If someone is interested, we return out special empty string.
<*/
char *str_empty() {
	return empty;
}
/*>
Apart from our special empty string, we also treat `NULL` as empty.
<*/
bool str_is_empty(const char *str) {
	return !str || str == empty;
}
/*>
We only free the memory for a string, if it is not empty.
<*/
char *str_free(char *str) {
	if (!str_is_empty(str)) free(str);
	return empty;
}
