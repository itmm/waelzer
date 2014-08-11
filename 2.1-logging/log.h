#if !defined(log_h)
#define log_h

	#include <stdarg.h>
	#include <stdbool.h>

	#define log(...) _do_log(__FILE__, __LINE__, "" __VA_ARGS__)

	#define return_unless(condition, value, ...) do { \
			if (!(condition)) { \
				_do_log(__FILE__, __LINE__, "" __VA_ARGS__); \
				return value; \
			} \
		} while(false) \

	void _do_log(const char *file, int line, const char *format,...);

	typedef void (*log_fn)(const char *file, int line, const char *format, va_list args);

	log_fn set_logger(log_fn logger);

#endif
