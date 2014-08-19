# Unit-Testing the Logger

It took a bit to make unit-tests for the Logger functions. The reason was the string list, that stores every log message (without file and line number).  We want to use this data structure, but to do that, it has to be present.

## Interface

Beside the headers, we declare a new log adapter, that records every log.

``` c
#if !defined(t_logger_h)
#define t_logger_h

	#if !defined(log_h)
		#include "../2.1-logging/log.h"
	#endif
	#if !defined(str_lst_h)
		#include "../2.4-string-list/str_lst.h"
	#endif

	void t_log_adapter_fn(const char *file, int line, const char *format, va_list args);
```

To access the messages, we can use the accessor to the string list. To reset the messages, a special clear function is provided.

``` c
	str_lst *t_log_adapter_get_messages();
	void t_log_adapter_clear_messages();

#endif
```
## Implementation

We need to include some headers and declare a static variable for the string list.

``` c
#include "t_logger.h"

#include "../2.4-string-list/str_lst.h"

#include <stdio.h>
#include <stdlib.h>

static str_lst *messages = NULL;
```

### Log Adapter

The log adapter stores the message in a buffer, creates the string list lazily and adds the string.

``` c
void t_log_adapter_fn(const char *file, int line, const char *format, va_list args) {
	static char buffer[100];
	vsnprintf(buffer, sizeof(buffer), format, args);
	buffer[sizeof(buffer) - 1] = 0;
	if (!messages) {
		messages = str_lst_create(1);
		return_unless(messages, , "can't allocate list");
	}
	str_lst_add(messages, buffer);
}
```

### String List handling

You can access the string list directly.

``` c
str_lst *t_log_adapter_get_messages() {
	return messages;
}
```

The clear function deletes the string list, so that no heap memory will be used.

``` c
void t_log_adapter_clear_messages() {
	str_lst_free(messages);
	messages = NULL;
}

```
## Unit-Tests

First we have some headers

``` c
#include "t_logger.h"

#include <stdarg.h>

#include "../2.2-unit-tests/unit.h"
#include "../2.3-strings/str.h"
```

A special tear down function will clear all messages:

``` c
static void teardown(void *context) {
	t_log_adapter_clear_messages();
}
```

The `assert_messages` function validates, that all messages are present, that are expected.

``` c
void assert_messages(int count, ...) {
	str_lst *messages = t_log_adapter_get_messages();
	assert_true(str_lst_count(messages) == count, "wrong count: %d != %d", str_lst_count(messages), count);
	char **cur = str_lst_begin(messages);
	va_list args;
	va_start(args, count);
	for (int i = count; i; --i) {
		char *exp = va_arg(args, char *);
		assert_str(*cur++, exp);
	}
	va_end(args);
}
```

### Tests
#### Log a simple message

``` c
void t_simple(void *context) {
	log("test %s", "abc");
	assert_messages(1, "test abc");
}
```

#### Log an empty message

``` c
void t_empty(void *context) {
	log();
	assert_messages(1, "");
}
```

### Fixture

``` c
int main(int argc, char **argv) {
	log_adapter_fn original = set_log_adapter(t_log_adapter_fn);
	run_test_ex(t_simple, NULL, NULL, teardown);
	run_test_ex(t_empty, NULL, NULL, teardown);
	set_log_adapter(original);
	t_log_adapter_clear_messages();
	unit_summary();
}
```
