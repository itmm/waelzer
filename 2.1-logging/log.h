#if !defined(log_h)
#define log_h

	#include <stdarg.h>
	#include <stdbool.h>

	#define log(...) _do_log(__FILE__, __LINE__, "" __VA_ARGS__)

	void _do_log(const char *file, int line, const char *format,...);

	typedef void (*log_adapter_fn)(const char *file, int line, const char *format, va_list args);

	log_adapter_fn set_log_adapter(log_adapter_fn log_adapter);
	log_adapter_fn disable_logging();
	log_adapter_fn set_default_logging();

	#define return_unless(condition, value, ...) do { \
			if (!(condition)) { \
				_do_log(__FILE__, __LINE__, "" __VA_ARGS__); \
				return value; \
			} \
		} while(false)

#endif
