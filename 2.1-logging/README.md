# Logging and Unit-Tests

Because Logging and Unit-Testing need each other, both are presented in this section.

## Logging

My view about Logging is the following: no function should ever crash. If something is wrong, it should tell the user via a log message, maybe tell the calling function and return. We assume, that the calling function will not inspect the return value, so all that is left from the error is the log message.

### log Macros

Two macros directly call the `_do_log` function and pass the current file and line number as parameters. The `log` macro can have an optional `printf`-style format list.

The `return_unless` macro has an additional `condition` and return `value`. If the `condition` evaluates to `true`, nothing happens. If it is `false`, the rest of the argument list will be used to form a log entry. After this logging, `value` will be returned from the function. If the function is `void`, you can leave `value` empty.

This macro will be used in lots of functions to check the validity of the current state or arguments.

### Need for an Adapter

I don't like the assert macro from C. It terminates the program. And I like the program to continue. I don't want to assure that everything is right, before I call a function and function validates again, that everything is right. DRY: Don't Repeat Yourself. And after all, it can be disabled in production code, so you still have to cope with the case of not crashing while doing so in the development environment.

I want that the function can be called with anything and if I inspect the return value, maybe I even want to suppress the logging. But that is an action triggered by the called of a function.

To get this feature, there is a log adapter, that can be switched and reset at runtime to stear the logging messages. If it is set to `NULL`, no logging will be performed.


## Unit-Tests

I love test-driven development, but it is hard to achieve. The most important part, is the testing framework. Here we use a very minimal framework, just for this project.

Each test is a function, that can get a context passed along. There are setup and tear down functions that can be invoked before and after any test.

If the test fails, we jump out of the test and continue with the next test.