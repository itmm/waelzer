#if !defined(unit_h)
#define unit_h

	#include <stdbool.h>
	#include <stdio.h>

	#if !defined(log_h)
		#include "../2.1-logging/log.h"
	#endif

	typedef void (*test_fn)(void *context);
	typedef void *(*setup_fn)(void *context);
	typedef void (*teardown_fn)(void *context);

	void run_test(test_fn test, void *context);
	void run_test_ex(test_fn test, void *context, setup_fn setup, teardown_fn teardown);

	void unit_summary();

	#define assert_true(condition, ...) do {\
			if (!(condition)) { \
				putchar('\n'); \
				set_default_logging(); \
				_do_log(__FILE__, __LINE__, "TEST FAILED: " __VA_ARGS__); \
				_exit_from_test(); \
			} \
		} while(false)

	#define assert_str(a, b) assert_true(!strcmp(a, b), "strings not equal \"%s\" != \"%s\"")

	void _exit_from_test();
#endif