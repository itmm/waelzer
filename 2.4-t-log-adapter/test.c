#include "t_logger.h"

#include "../2.2-unit-tests/unit.h"
#include "../2.3-strings/str.h"

#include <string.h>

static log_adapter_fn original_adapter;

static void *setup(void *context) {
	t_log_adapter_clear_messages();
	return context;
}

void assert_messages(const char *expected) {
	char *messages = t_log_adapter_copy_messages();
	assert_true(!strcmp(messages, expected), "messages differ: '%s' != '%s'", messages, expected);
	str_free(messages);
}

void t_simple(void *context) {
	log("test %s", "abc");
	assert_messages("test abc");
}

void t_empty(void *context) {
	log();
	assert_messages("");
}

int main(int argc, char **argv) {
	original_adapter = set_log_adapter(t_log_adapter_fn);
	run_test_ex(t_simple, NULL, setup, NULL);
	run_test_ex(t_empty, NULL, setup, NULL);
	set_log_adapter(original_adapter);
	t_log_adapter_clear_messages();
	unit_summary();
}
