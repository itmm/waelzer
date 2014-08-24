# String Utilities

The string utilities has a method to concatenate any number of strings.

``` c
#if !defined(str_h)
#define str_h

	#include <stdbool.h>

	char *str_cons(int count, ...);
```

Also there is a special empty string, that will be returned on error.  You can check, if a string is `NULL` or the special empty string.

``` c
	char *str_empty();
	bool str_is_empty(const char *str);
```

A new free function for strings handles gracefully the cases of `NULL` and the special empty string (which also must not be freed).

This function returns a value to the empty string, so we can assign the variable that we free with an empty default by assigning it the value of this function.

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

The special empty string contains just one byte: the null terminator. We don't treat any empty string as special. It must have the same address as the special empty string to qualify for special treatment.

``` c
static char empty[1] = { 0 };
```

### Concatenating Strings

To concatenate the strings, we build two `va_list` of the variable arguments.  The first will be used to calculate the length of the resulting string. The second will be used in the copy phase in the function `copy_args`.

``` c
static void copy_args(char *result, int count, va_list args);

char *str_cons(int count, ...) {
	return_unless(count >= 0, empty, "count must not be negative");
	if (!count) { return empty; }

	va_list args1;
	va_list args2;
	va_start(args1, count);
	va_copy(args2, args1);
```

The calculation of the length is straight forward. We only have to watch out for `NULL` strings, that will be silently ignored.

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
```

The `copy_args` function may throw an error. We put it in a different function, so we can free the second argument list, even, if an error is raised.

``` c
	copy_args(result, count, args2);
	va_end(args2);

	return result ? : empty;
}
```

In the copy phase, we copy the current argument and advance the pointer afterwards.

``` c
static void copy_args(char *result, int count, va_list args) {
	return_unless(result,, "can't alloc result string");
	return_unless(count >= 0,, "count must not be negative");

	for (int i = count; i; --i) {
		const char *current = va_arg(args, const char *);
		size_t len = current ? strlen(current) : 0;
		memcpy(result, current, len);
		result += len;
	}
```

Don't forget to add the terminating null byte.

``` c
	*result = 0;
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

A special tear down function can be used to assure, that the memory of allocated strings is freed, even when the test failed.

``` c
static char *str = NULL;

static void teardown(void *context) {
	str = str_free(str);
}
```

### Concatenation tests
#### Can we concatenate a couple of strings?

``` c
static void t_simple(void *context) {
	str = str_cons(3, "a", "b", "c");
	assert_str(str, "abc");
}
```

#### Even if some entries are `NULL`?

``` c
static void t_null_entries(void *context) {
	str = str_cons(5, "a", NULL, "b", NULL, "c");
	assert_str(str, "abc");
}
```

#### How about zero entries?

``` c
static void t_empty(void *context) {
	str = str_cons(0);
	assert_str(str, "");
}
```

#### Or only one `NULL` entry?

``` c
static void t_only_NULL(void *context) {
	str = str_cons(1, NULL);
	assert_str(str, "");
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
	run_test_ex(t_simple, NULL, NULL, teardown);
	run_test_ex(t_null_entries, NULL, NULL, teardown);
	run_test_ex(t_empty, NULL, NULL, teardown);
	run_test_ex(t_only_NULL, NULL, NULL, teardown);
	run_test(t_free_NULL, NULL);
	run_test(t_free_empty, NULL);
	unit_summary();
}
```
