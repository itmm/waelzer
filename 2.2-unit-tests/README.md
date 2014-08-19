# Unit-Tests

I love test-driven development, but it is hard to achieve. The most important part, is the testing framework. Here we use a very minimal framework, just for this project.

Each test is a function, that can get a context passed along. There are setup and tear down functions that can be invoked before and after any test.

If the test fails, we jump out of the test and continue with the next test.

The following code shows all, that should be needed to run a test:

``` c
#include "unit.h"

void t_trivial(void *context) {}

int main(int argc, char **argv) {
	run_test(t_trivial, NULL);
	unit_summary();
}
```
## The Unit-Test Interface

First we need some system headers and our logging.

``` c
#if !defined(unit_h)
#define unit_h

	#if !defined(log_h)
		#include "../2.1-logging/log.h"
	#endif

	#include <stdio.h>
	#include <string.h>
```

We use three kind of functions.

The `test_fn` gets a `context` as a parameter. This context can be passed to the call or created by the setup function. It should perform the test and use the assertion functions describe below to assure the correctness of the test.

The `setup_fn` will be called before the test. It can be used to create the scenario, in which the test is performed. It is passed the `context` from the test invocation, but it returns a `context` that will be passed to the test and tear down functions. The setup function is optional and `NULL` can be passed instead.

The `teardown_fn` is also optional. It will be called after each test is run (no matter, if it succeeded or failed) to tear down the scenario created in the setup function.

``` c
	typedef void (*test_fn)(void *context);
	typedef void *(*setup_fn)(void *context);
	typedef void (*teardown_fn)(void *context);

```

### Running the tests

There are two ways to run a test. The easy one is passed a test function and a context (which can be `NULL`). The full version also gets the setup and teardown functions as parameters.

``` c
	void run_test(test_fn test, void *context);
	void run_test_ex(test_fn test, void *context, setup_fn setup, teardown_fn teardown);
```

After the tests are performed, you can invoke `unit_summary` to print a short summary. If some tests failed, the function will call `exit()` with a failure code.

``` c
	void unit_summary();

```

### Assertions

The assertions are macros, that test a condition. If it fails, it log an error and mark the test as failed by calling `_exit_from_test`.

``` c
	void _exit_from_test();

	#define assert_true(condition, ...) do {\
			if (!(condition)) { \
				putchar('\n'); \
				set_default_logging(); \
				_do_log(__FILE__, __LINE__, "TEST FAILED: " __VA_ARGS__); \
				_exit_from_test(); \
			} \
		} while(false)
```

For comparing strings there is a short cut, that provides a log message.

``` c
	#define assert_str(a, b) assert_true(!strcmp(a, b), "strings not equal \"%s\" != \"%s\"")

#endif
```
## Implementing Unit-Tests

First we need a couple of includes.

``` c
#include "unit.h"

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
```

In two variable we keep track of tests performed and the number of failed tests.

``` c
static int _run = 0;
static int _fails = 0;
```

For the `_exit_test` function we set up a long jump, that can be called from inside the test function to exit the test with a failure.

``` c
static jmp_buf test_exit;
```

### Running a test

To run a test we first call the setup function, if it is present.

Then we configure the long jump, call the test function and write a `.` (that is the reason, that the assertion macro writes a newline before logging its message). 

If the long jump was triggered, we write a `-` instead and increase the counter of failed tests.

Afterwards a tear down function is called, if it is present.

``` c
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
```

The simple invocation calls the complete function without setup and tear down functions.

``` c
void run_test(test_fn test, void *context) {
	run_test_ex(test, context, NULL, NULL);
}
```

### Printing the summary

The summary handles two cases. If tests failed, the test runner will be terminated with a non-null exit code.

``` c
void unit_summary() {
	if (_fails) {
		printf("\n\nFAILED (%d from %d tests failed)\n", _fails, _run);
		exit(EXIT_FAILURE);
	} else {
		printf("\n\nok (%d tests)\n", _run);
	}
}
```

### Aborting tests

To abort a test, we simply invoke the long jump.

``` c
void _exit_from_test() {
	longjmp(test_exit, 1);
}

```
