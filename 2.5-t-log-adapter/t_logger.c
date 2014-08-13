/*>
## Implementation

We need to include some headers and declare a static variable for the string list.
<*/
#include "t_logger.h"

#include "../2.4-string-list/str_lst.h"

#include <stdio.h>
#include <stdlib.h>

static str_lst *messages = NULL;
/*>
### Log Adapter

The log adapter stores the message in a buffer, creates the string list lazyly and
adds the string.
<*/
void t_log_adapter_fn(const char *file, int line, const char *format, va_list args) {
	static char buffer[100];
	vsnprintf(buffer, sizeof(buffer), format, args);
	buffer[sizeof(buffer) - 1] = 0;
	if (!messages) {
		messages = str_lst_create(1);
		return_unless(messages, , "can't allocate list");
	}
	str_lst_add(messages, buffer);
}
/*>
### String List handling

You can access the string list directly.
<*/
str_lst *t_log_adapter_get_messages() {
	return messages;
}
/*>
The clear function deletes the string list, so that no heap memory will be used.
<*/
void t_log_adapter_clear_messages() {
	str_lst_free(messages);
	messages = NULL;
}

