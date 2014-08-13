#Logging
## First define the target

We need a method to log statements like the following:
``` c
#include "log.h"

int main(int argc, char **argv) {
	log("all is %s", "ok");
}
```
If that is all, we could use `fprintf` instead. But there is more! There
should be a possibility to disable the logging on a call by call basis.

And the output should include the file and the line number where the log
occured:

```
main.c:4 all is ok
```

While we can do this also with a direct function call, it is tedious to
do so in C. And we want logging to super easy, so it will be used more
often and doesn't cludder the code too much.
## Describing the Interface

In this part we describe, how the logging framework can be used. Then we
describe, how it is implemented. First we need some standard includes:
``` c
#if !defined(log_h)
#define log_h

	#include <stdarg.h>
	#include <stdbool.h>
```
The basic logging is done with the method `_do_log`, but this method is
seldom called. It has the file name and line number as parameters.
Most of the time it will be called by the `log` macro, that inserts these
two parameters.

Additional parameters can be provided as in the `printf` function.
``` c
	void _do_log(const char *file, int line, const char *format,...);
	#define log(...) _do_log(__FILE__, __LINE__, "" __VA_ARGS__)
```
### Fast function exit

The `return_unless` macro has an additional `condition` and return `value`. 
If the `condition` evaluates to `true`, nothing happens. If it is `false`, 
the rest of the argument list will be used to form a log entry. After this 
logging, `value` will be returned from the function. If the function is `void`, 
you can leave `value` empty.

This macro will be used in lots of functions to check the validity of the 
current state or arguments.
``` c
	#define return_unless(condition, value, ...) do { \
			if (!(condition)) { \
				_do_log(__FILE__, __LINE__, "" __VA_ARGS__); \
				return value; \
			} \
		} while(false)

```
### The Log Adapter

I don't like the assert macro from C. It terminates the program. And I like the program to continue. I don't want to assure that everything is right, before I call a function and function validates again, that everything is right. DRY: Don't Repeat Yourself. And after all, it can be disabled in production code, so you still have to cope with the case of not crashing while doing so in the development environment.

I want that the function can be called with anything and if I inspect the return value, 
maybe I even want to suppress the logging. But that is an action triggered by the called of a function.

To get this feature, there is a log adapter function, that is called by the `_do_log` function. It can be changed
globally. The setter functions return the old log adapter to restore the old log behaviour.

It is also possible to disable logging or reset the default behaviour.
``` c
	typedef void (*log_adapter_fn)(const char *file, int line, const char *format, va_list args);

	log_adapter_fn set_log_adapter(log_adapter_fn log_adapter);
	log_adapter_fn disable_logging();
	log_adapter_fn set_default_logging();

#endif
```
## Implementing the Logging

First we need to include the interface and the io header.
``` c
#include "log.h"

#include <stdio.h>
```
### The default log adapter

The logging takes place in the default log adapter. It writes the current
file and line number and the log message to `stderr`. Also a newline is
written afterwards
``` c
static void default_log_adapter(const char *file, int line, const char *format, va_list args) {
	fprintf(stderr, "%s:%d ", file, line);
	vfprintf(stderr, format, args);
	fputc('\n', stderr);
}
```
The current adapter is set to the default adapter
``` c
static log_adapter_fn adapter = default_log_adapter;
```
### Do the Logging

The function `_do_log` converts the variable arguments into a `va_list`
and calls the adapter, if one is present.
``` c
void _do_log(const char *file, int line, const char *format,...) {
	if (adapter) {
		va_list args;
		va_start(args, format);
		adapter(file, line, format, args);
		va_end(args);
	}
}
```
### Changing the log adapter

The function `set_log_adapter` can be used to change the
current adapter. The previous adapter will be returned.
``` c
log_adapter_fn set_log_adapter(log_adapter_fn log_adapter) {
	log_adapter_fn old = adapter;
	adapter = log_adapter;
	return old;
}
```
Logging can be disabled, by setting the adapter to `NULL`.
``` c
log_adapter_fn disable_logging() {
	return set_log_adapter(NULL);
}
```
The `default_log_adapter` can be set. It was either this
function, or make the default adapter public. I think, this
is the cleaner way to hide the details about the default
function.
``` c
log_adapter_fn set_default_logging() {
	return set_log_adapter(default_log_adapter);
}
```
