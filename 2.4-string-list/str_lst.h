/*>
# String Lists

Often we need a list of strings. This section provides an abstract data type.

## Interface

First we define an abstract data type and functions to create and destroy instances of this data type. You can pass the constructor the minimal initial capacity to allocate.
<*/
#if !defined(str_lst_h)
#define str_lst_h

	#include <stdbool.h>

	typedef struct str_lst str_lst;

	str_lst *str_lst_create(int min_capacity);
	void str_lst_free(str_lst *sl);
/*>
### Accessors

The accessor functions let you know how many strings are stored in the list, and get a pointer to the first string and behind the last string. The string pointers are stored in one block of
memory.
<*/
	int str_lst_count(str_lst *sl);
	char **str_lst_begin(str_lst *sl);
	char **str_lst_end(str_lst *sl);
/*>
### Adding Strings

There are two methods to add a string. The `str_lst_add` function stores a copy of the string in the list.

The function `str_lst_add_and_consume` stores the string passed in the list and frees its memory, when the list is freed.
<*/
	bool str_lst_add(str_lst *sl, const char *str);
	bool str_lst_add_and_consume(str_lst *sl, char *str);

#endif
