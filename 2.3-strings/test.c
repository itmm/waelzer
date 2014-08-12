#include "str.h"

#include "../2.2-unit-tests/unit.h"

#include <string.h>

void t_simple(void *context) {
	char *result = str_cons(3, "a", "b", "c");
	assert_str(result, "abc");
	str_free(result);
}

void t_null_entries(void *context) {
	char *result = str_cons(5, "a", NULL, "b", NULL, "c");
	assert_str(result, "abc");
	str_free(result);
}

void t_empty(void *context) {
	char *result = str_cons(0);
	assert_str(result, "");
	str_free(result);
}

void t_only_NULL(void *context) {
	char *result = str_cons(1, NULL);
	assert_str(result, "");
	str_free(result);
}

void t_free_NULL(void *context) {
	str_free(NULL);
}

void t_free_empty(void *context) {
	str_free(str_empty());
}

int main(int argc, char **argv) {
	run_test(t_simple, NULL);
	run_test(t_null_entries, NULL);
	run_test(t_empty, NULL);
	run_test(t_only_NULL, NULL);
	run_test(t_free_NULL, NULL);
	run_test(t_free_empty, NULL);
	unit_summary();
}
