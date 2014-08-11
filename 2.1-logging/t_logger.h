#if !defined(t_logger_h)
#define t_logger_h

	#if !defined(log_h)
		#include "log.h"
	#endif

	void t_log_adapter_fn(const char *file, int line, const char *format, va_list args);

	char *t_log_adapter_copy_messages();
	void t_log_adapter_clear_messages();

#endif
