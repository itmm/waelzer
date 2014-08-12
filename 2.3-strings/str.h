#if !defined(str_h)
#define str_h

	#include <stdbool.h>

	char *str_cons(int count, ...);
	char *str_free(char *str);
	char *str_empty();

	bool str_is_empty(const char *str);

#endif
