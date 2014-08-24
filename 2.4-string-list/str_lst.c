/*>
## Implementation

First we need a couple of headers and declare the `MIN` and `MAX` macros (yes, we need both).
<*/
#include "str_lst.h"

#include "../2.1-logging/log.h"
#include "../2.3-strings/str.h"

#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
/*>
## data type

The `str_lst` contains three pointers:

* `begin` points to the beginning of the list,
* `end` points behind the last added element,
* `capacity` points behind the last allocated entry.

Every time the relation `begin` &leq; `end` &leq; `capacity` must hold.
<*/
struct str_lst {
	char **begin;
	char **end;
	char **capacity;
};
/*>
### Creating a new list

The constructor allocates two chunks of memory: for the list structure itself and to store the string pointers. The minimum capacity is adjusted to a sensible size.

To avoid a memory leak while using the `return_unless` macro, the function is split in two. The adjustment of the pointers will check and fail, if the second memory chunk could not be allocated. If this is the case, the first chunk has to be freed as well.
<*/
static bool str_lst_adjust_pointers(str_lst *sl, int capacity);

str_lst *str_lst_create(int min_capacity) {
	int capacity = MAX(min_capacity, 8);
	str_lst *sl = malloc(sizeof(str_lst));
	return_unless(sl, NULL, "no memory for struct");
	sl->begin = malloc(sizeof(char **) * capacity);
	if (!str_lst_adjust_pointers(sl, capacity)) {
		free(sl);
		sl = NULL;
	}

	return sl;
}
/*>
The pointer adjustment checks both memory blocks and adjusts the other pointers of the `str_lst` structure.
<*/
static bool str_lst_adjust_pointers(str_lst *sl, int capacity) {
	return_unless(sl, false, "no list");
	return_unless(sl->begin, false, "no memory for buffer");
	sl->end = sl->begin;
	sl->capacity = sl->begin + capacity;
	return true;
}

/*>
### Free the list

First every string stored in the list is freed. Then the buffer is freed.  And lastly the list is freed.

This function returns `NULL`, so the return value of this function can be assigned to variable passed, so it doesn't point to a freed memory location.
<*/
str_lst *str_lst_free(str_lst *sl) {
	if (!sl) { return NULL; }
	if (sl->begin) {
		for (char **i = sl->begin; i != sl->end; ++i) { str_free(*i); }
		free(sl->begin);
	}
	free(sl);
	return NULL;
}

/*>
### Accessors

The count is simply the difference between end and begin
<*/
int str_lst_count(str_lst *sl) {
	if (!sl) { return 0; }
	return sl->end - sl->begin;
}
/*>
Begin and end are taken directly from the data type.
<*/
char **str_lst_begin(str_lst *sl) {
	return sl ? sl->begin : NULL;
}

char **str_lst_end(str_lst *sl) {
	return sl ? sl->end : NULL;
}
/*>
### Adding Strings

The convenience function duplicates the string and calls the consuming function.
<*/
bool str_lst_add(str_lst *sl, const char *str) {
	return str_lst_add_and_consume(sl, str_cons(1, str));
}
/*>
In the consuming function we do the heavy lifting, if the capacity is exceeded. Then we double the capacity until we grow so big, that we switch to a linear growth.

We reallocate the buffer for the new size, adjust the pointers and add the element. If we can't reallocate the buffer, nothing will be changed.
<*/
bool str_lst_add_and_consume(str_lst *sl, char *str) {
	return_unless(sl, false, "no list");
	if (sl->end == sl->capacity) {
		int old_count = sl->capacity - sl->begin;
		int new_count = old_count + MIN(old_count, 1024);
		void *newBuffer = realloc(sl->begin, new_count * sizeof(char **));
		return_unless(newBuffer, false, "can't expand buffer");
		sl->begin = newBuffer;
		sl->end = sl->begin + old_count;
		sl->capacity = sl->begin + new_count;
	}
	*sl->end++ = str;
	return true;
}

