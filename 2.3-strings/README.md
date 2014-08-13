# String Utilities

The string utilities has a method to concatenate any number of strings.
``` c
#if !defined(str_h)
#define str_h

	#include <stdbool.h>

	char *str_cons(int count, ...);
```
Also there is a special empty string, that will be returned on error.
You can check, if a string is `NULL` or the special empty string.
``` c
	char *str_empty();
	bool str_is_empty(const char *str);
```
A new free function for strings handles gracefully the cases of `NULL`
and the special empty string (which also must not be freed).
``` c
	char *str_free(char *str);

#endif
```
## The implementation

First we need a couple of headers.
``` c
#include "str.h"

#include "../2.1-logging/log.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
```
The special empty string contains just one byte: the null terminator. We
don't treat any empty string as special. It must have the same address as
the special empty string to qualify for special treatment.
``` c
static char empty[1] = { 0 };
```
### Concatenating Strings

To concatenate the strings, we build two `va_list` of the variable arguments.
The first will be used to calculate the length of the resulting string. The
second will be used in the copy phase.
``` c
char *str_cons(int count, ...) {
	return_unless(count >= 0, empty, "count must not be negative");
	if (!count) { return empty; }

	va_list args1;
	va_list args2;
	va_start(args1, count);
	va_copy(args2, args1);
```
The calculation of the length is straight forward. We only have to watch out
for `NULL` strings, that will be silently ignored.
``` c
	size_t length = 0;
	for (int i = count; i; --i) {
		const char *current = va_arg(args1, const char *);
		if (current) length += strlen(current);
	}
	va_end(args1);
```
The result buffer must be one byte bigger to store the terminating null byte.
``` c
	char *result = malloc(length + 1);
	if (!result) {
		va_end(args2);
		return_unless(false, empty, "Can't alloc result string");
	}
```
In the copy phase, we keep a pointer to the current end of the string and
adavance it after each copy.
``` c
	char *tail = result;
	for (int i = count; i; --i) {
		const char *current = va_arg(args2, const char *);
		size_t len = current ? strlen(current) : 0;
		memcpy(tail, current, len);
		tail += len;
	}
```
Don't forget to add the terminating null byte.
``` c
	*tail = 0;
	va_end(args2);

	return result;
}
```
### Special empty string

If someone is interested, we return out special empty string.
``` c
char *str_empty() {
	return empty;
}
```
Apart from our special empty string, we also treat `NULL` as empty.
``` c
bool str_is_empty(const char *str) {
	return !str || str == empty;
}
```
We only free the memory for a string, if it is not empty.
``` c
char *str_free(char *str) {
	if (!str_is_empty(str)) free(str);
	return empty;
}
```
## Unit-Tests

We need some headers.
``` c
#include "str.h"

#include "../2.2-unit-tests/unit.h"
```
### Concatenation tests
#### Can we concatenate a couple of strings?
``` c
void t_simple(void *context) {
	char *result = str_cons(3, "a", "b", "c");
	assert_str(result, "abc");
	str_free(result);
}
```
#### Even if some entries are `NULL`?
``` c
void t_null_entries(void *context) {
	char *result = str_cons(5, "a", NULL, "b", NULL, "c");
	assert_str(result, "abc");
	str_free(result);
}
```
#### How about zero entries?
``` c
void t_empty(void *context) {
	char *result = str_cons(0);
	assert_str(result, "");
	str_free(result);
}
```
#### Or only one `NULL` entry?
``` c
void t_only_NULL(void *context) {
	char *result = str_cons(1, NULL);
	assert_str(result, "");
	str_free(result);
}
```
### Freeing tests
#### Can we free `NULL`?
``` c
void t_free_NULL(void *context) {
	str_free(NULL);
}
```
#### Or the empty string?
``` c
void t_free_empty(void *context) {
	str_free(str_empty());
}
```
### Setup
``` c
int main(int argc, char **argv) {
	run_test(t_simple, NULL);
	run_test(t_null_entries, NULL);
	run_test(t_empty, NULL);
	run_test(t_only_NULL, NULL);
	run_test(t_free_NULL, NULL);
	run_test(t_free_empty, NULL);
	unit_summary();
}
```
