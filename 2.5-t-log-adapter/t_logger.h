/*>
# Unit-Testing the Logger

It took a bit to make unit-tests for the Logger functions. The reason was the string list, that stores every log message (without file and line number).  We want to use this data structure, but to do that, it has to be present.

## Interface

Beside the headers, we declare a new log adapter, that records every log.
<*/
#if !defined(t_logger_h)
#define t_logger_h

	#if !defined(log_h)
		#include "../2.1-logging/log.h"
	#endif
	#if !defined(str_lst_h)
		#include "../2.4-string-list/str_lst.h"
	#endif

	void t_log_adapter_fn(const char *file, int line, const char *format, va_list args);
/*>
To access the messages, we can use the accessor to the string list. To reset the messages, a special clear function is provided.
<*/
	str_lst *t_log_adapter_get_messages();
	void t_log_adapter_clear_messages();

#endif
