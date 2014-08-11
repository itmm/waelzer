#include "unit.h"

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static int _run = 0;
static int _fails = 0;
static jmp_buf test_exit;

void run_test(test_fn test, void *context) {
	run_test_ex(test, context, NULL, NULL);
}

void run_test_ex(test_fn test, void *context, setup_fn setup, teardown_fn teardown) {
	return_unless(test, , "no test function");
	if (setup) { context = setup(context); }
	++_run;
	if (!setjmp(test_exit)) {
		test(context);
		putchar('.');
	} else {
		++_fails;
		putchar('-');
	}
	if (teardown) { teardown(context); }
}

void unit_summary() {
	if (_fails) {
		printf("\n\nFAILED (%d from %d tests failed)\n", _fails, _run);
		exit(EXIT_FAILURE);
	} else {
		printf("\n\nok (%d tests)\n", _run);
	}
}

void _exit_from_test() {
	longjmp(test_exit, 1);
}

