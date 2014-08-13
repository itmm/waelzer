/*>
## Unit-Tests

First we have some headers
<*/
#include "t_logger.h"

#include <stdarg.h>

#include "../2.2-unit-tests/unit.h"
#include "../2.3-strings/str.h"
/*>
A special tear down function will clear all messages
<*/
static void teardown(void *context) {
	t_log_adapter_clear_messages();
}
/*>
The `assert_messages` function validates, that all messages are
present, that are expected.
<*/
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
/*>
### Tests
#### Log a simple message
<*/
void t_simple(void *context) {
	log("test %s", "abc");
	assert_messages(1, "test abc");
}
/*>
#### Log an empty message
<*/
void t_empty(void *context) {
	log();
	assert_messages(1, "");
}
/*>
### Fixture
<*/
int main(int argc, char **argv) {
	log_adapter_fn original = set_log_adapter(t_log_adapter_fn);
	run_test_ex(t_simple, NULL, NULL, teardown);
	run_test_ex(t_empty, NULL, NULL, teardown);
	set_log_adapter(original);
	t_log_adapter_clear_messages();
	unit_summary();
}
