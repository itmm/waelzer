/*>
# String Utilities

The string utilities has a method to concatenate any number of strings.
<*/
#if !defined(str_h)
#define str_h

	#include <stdbool.h>

	char *str_cons(int count, ...);
/*>
Also there is a special empty string, that will be returned on error.
You can check, if a string is `NULL` or the special empty string.
<*/
	char *str_empty();
	bool str_is_empty(const char *str);
/*>
A new free function for strings handles gracefully the cases of `NULL`
and the special empty string (which also must not be freed).

This function returns a value to the empty string, so we can assign
the variable that we free with an empty default by assigning it the
value of this function.
<*/
	char *str_free(char *str);

#endif
