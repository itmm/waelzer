/*>
## Describing the Interface

In this part we describe, how the logging framework can be used. Then we
describe, how it is implemented. First we need some standard includes:
<*/
#if !defined(log_h)
#define log_h

	#include <stdarg.h>
	#include <stdbool.h>
/*>
The basic logging is done with the method `_do_log`, but this method is
seldom called. It has the file name and line number as parameters.
Most of the time it will be called by the `log` macro, that inserts these
two parameters.

Additional parameters can be provided as in the `printf` function.
<*/
	void _do_log(const char *file, int line, const char *format,...);
	#define log(...) _do_log(__FILE__, __LINE__, "" __VA_ARGS__)
/*>
### Fast function exit

The `return_unless` macro has an additional `condition` and return `value`. 
If the `condition` evaluates to `true`, nothing happens. If it is `false`, 
the rest of the argument list will be used to form a log entry. After this 
logging, `value` will be returned from the function. If the function is `void`, 
you can leave `value` empty.

This macro will be used in lots of functions to check the validity of the 
current state or arguments.
<*/
	#define return_unless(condition, value, ...) do { \
			if (!(condition)) { \
				_do_log(__FILE__, __LINE__, "" __VA_ARGS__); \
				return value; \
			} \
		} while(false)

/*>
### The Log Adapter

I don't like the assert macro from C. It terminates the program. And I like the program to continue. I don't want to assure that everything is right, before I call a function and function validates again, that everything is right. DRY: Don't Repeat Yourself. And after all, it can be disabled in production code, so you still have to cope with the case of not crashing while doing so in the development environment.

I want that the function can be called with anything and if I inspect the return value, 
maybe I even want to suppress the logging. But that is an action triggered by the called of a function.

To get this feature, there is a log adapter function, that is called by the `_do_log` function. It can be changed
globally. The setter functions return the old log adapter to restore the old log behaviour.

It is also possible to disable logging or reset the default behaviour.
<*/
	typedef void (*log_adapter_fn)(const char *file, int line, const char *format, va_list args);

	log_adapter_fn set_log_adapter(log_adapter_fn log_adapter);
	log_adapter_fn disable_logging();
	log_adapter_fn set_default_logging();

#endif
