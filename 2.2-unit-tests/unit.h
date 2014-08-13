/*>
## The Unit-Test Interface

First we need some system headers and our logging.
<*/
#if !defined(unit_h)
#define unit_h

	#if !defined(log_h)
		#include "../2.1-logging/log.h"
	#endif

	#include <stdio.h>
	#include <string.h>
/*>
We use three kind of functions.

The `test_fn` gets a `context` as a parameter. This context can be passed
to the call or created by the setup function. It should perform the test
and use the assertion functions describe below to assure the correctness
of the test.

The `setup_fn` will be called before the test. It can be used to create
the scenario, in which the test is performed. It is passed the `context`
from the test invocation, but it returns a `context` that will be passed
to the test and tear down functions. The setup function is optional and
`NULL` can be passed instead.

The `teardown_fn` is also optional. It will be called after each test is
run (no matter, if it succeeded or failed) to tear down the scenario
created in in the setup function.
<*/
	typedef void (*test_fn)(void *context);
	typedef void *(*setup_fn)(void *context);
	typedef void (*teardown_fn)(void *context);

/*>
### Running the tests

There are two ways to run a test. The easy one is passed a test function
and a context (which can be `NULL`). The full version also gets the
setup and teardown functions as parameters.
<*/
	void run_test(test_fn test, void *context);
	void run_test_ex(test_fn test, void *context, setup_fn setup, teardown_fn teardown);
/*>
After the tests are performed, you can invoke `unit_summary` to print a
short summary. If some tests failed, the function will call `exit()` with
a failure code.
<*/
	void unit_summary();

/*>
### Assertions

The assertions are macros, that test a condition. If it fails, it log an
error and mark the test as failed by calling `_exit_from_test`.
<*/
	void _exit_from_test();

	#define assert_true(condition, ...) do {\
			if (!(condition)) { \
				putchar('\n'); \
				set_default_logging(); \
				_do_log(__FILE__, __LINE__, "TEST FAILED: " __VA_ARGS__); \
				_exit_from_test(); \
			} \
		} while(false)
/*>
For comparing strings there is a short cut, that provides a log message.
<*/
	#define assert_str(a, b) assert_true(!strcmp(a, b), "strings not equal \"%s\" != \"%s\"")

#endif
