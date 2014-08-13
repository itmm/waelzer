/*>
## Implementing Unit-Tests

First we need a couple of includes.
<*/
#include "unit.h"

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
/*>
In two variable we keep track of tests performed and the number
of failed tests.
<*/
static int _run = 0;
static int _fails = 0;
/*>
For the `_exit_test` function we set up a long jump, that can be
called from inside the test function to exit the test with a failure.
<*/
static jmp_buf test_exit;
/*>
### Running a test

To run a test we first call the setup function, if it is present.

Then we configure the long jump, call the test function and write a `.` (that is the
reason, that the assert macro writes a newline before logging its message). 

If the long jump was triggered, we write a `-` instead and increase the the counter of
failed tests.

Afterwards a tear down function is called, if it is present.
<*/
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
/*>
The simple invocation calls the complete function without setup and tear down functions.
<*/
void run_test(test_fn test, void *context) {
	run_test_ex(test, context, NULL, NULL);
}
/*>
### Printing the summary

The summary handles two cases. If tests failed, the test runner will be terminated
with a non-null exit code.
<*/
void unit_summary() {
	if (_fails) {
		printf("\n\nFAILED (%d from %d tests failed)\n", _fails, _run);
		exit(EXIT_FAILURE);
	} else {
		printf("\n\nok (%d tests)\n", _run);
	}
}
/*>
### Aborting tests

To abort a test, we simply invoce the long jump.
<*/
void _exit_from_test() {
	longjmp(test_exit, 1);
}

