#include "str.h"

#include "../2.2-unit-tests/unit.h"

#include <string.h>

void t_simple(void *context) {
	char *result = str_cons(3, (const char *[]) {"a", "b", "c"});
	const char *expected = "abc";
	assert_true(!strcmp(expected, result), "don't mach '%s' != '%s'", result, expected);
	str_free(result);
}

int main(int argc, char **argv) {
	run_test(t_simple, NULL);
	unit_summary();
}
