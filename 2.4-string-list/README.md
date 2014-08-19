# String Lists

Often we need a list of strings. This section provides an abstract data type.

## Interface

First we define an abstract data type and functions to create and destroy instances of this data type. You can pass the constructor the minimal initial capacity to allocate.

``` c
#if !defined(str_lst_h)
#define str_lst_h

	#include <stdbool.h>

	typedef struct str_lst str_lst;

	str_lst *str_lst_create(int min_capacity);
	void str_lst_free(str_lst *sl);
```

### Accessors

The accessor functions let you know how many strings are stored in the list, and get a pointer to the first string and behind the last string. The string pointers are stored in one block of
memory.

``` c
	int str_lst_count(str_lst *sl);
	char **str_lst_begin(str_lst *sl);
	char **str_lst_end(str_lst *sl);
```

### Adding Strings

There are two methods to add a string. The `str_lst_add` function stores a copy of the string in the list.

The function `str_lst_add_and_consume` stores the string passed in the list and frees its memory, when the list is freed.

``` c
	bool str_lst_add(str_lst *sl, const char *str);
	bool str_lst_add_and_consume(str_lst *sl, char *str);

#endif
```
## Implementation

First we need a couple of headers and declare the `MIN` and `MAX` macros (yes, we need both).

``` c
#include "str_lst.h"

#include "../2.1-logging/log.h"
#include "../2.3-strings/str.h"

#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
```

## data type

The `str_lst` contains three pointers:

* `begin` points to the beginning of the list,
* `end` points behind the last added element,
* `capacity` points behind the last allocated entry.

Every time the relation `begin` &leq; `end` &leq; `capacity` must hold.

``` c
struct str_lst {
	char **begin;
	char **end;
	char **capacity;
};
```

### Creating a new list

The constructor allocates two chunks of memory: for the list structure itself and to store the string pointers. The minimum capacity is adjusted to a sensible size.

``` c
str_lst *str_lst_create(int min_capacity) {
	int capacity = MAX(min_capacity, 8);
	str_lst *sl = malloc(sizeof(str_lst));
	return_unless(sl, NULL, "no memory for struct");
	sl->begin = malloc(sizeof(char **) * capacity);
	if (!sl->begin) {
		free(sl);
		return_unless(false, NULL, "no memory for buffer");
	}
	sl->end = sl->begin;
	sl->capacity = sl->begin + capacity;

	return sl;
}
```

### Free the list

First every string stored in the list is freed. Then the buffer is freed.  And lastly the list is freed.

``` c
void str_lst_free(str_lst *sl) {
	if (!sl) { return; }
	if (sl->begin) {
		for (char **i = sl->begin; i != sl->end; ++i) { str_free(*i); }
		free(sl->begin);
	}
	free(sl);
}

```

### Accessors

The count is simply the difference between end and begin

``` c
int str_lst_count(str_lst *sl) {
	if (!sl) { return 0; }
	return sl->end - sl->begin;
}
```

Begin and end are taken directly from the data type.

``` c
char **str_lst_begin(str_lst *sl) {
	return sl ? sl->begin : NULL;
}

char **str_lst_end(str_lst *sl) {
	return sl ? sl->end : NULL;
}
```

### Adding Strings

The convenience function duplicates the string and calls the consuming function.

``` c
bool str_lst_add(str_lst *sl, const char *str) {
	return str_lst_add_and_consume(sl, str_cons(1, str));
}
```

In the consuming function we do the heavy lifting, if the capacity is exceeded. Then we double the capacity until we grow so big, that we switch to a linear growth.

We reallocate the buffer for the new size, adjust the pointers and add the element. If we can't reallocate the buffer, nothing will be changed.

``` c
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

```
## Unit-Tests

First we need some headers.

``` c
#include "str_lst.h"

#include "../2.2-unit-tests/unit.h"
#include "../2.3-strings/str.h"

#include <string.h>
```

### General setup

The variable `sl` hold one list, that is managed by the `setup` and `teardown` functions.

``` c
static str_lst *sl;

static void *setup(void *context) {
	sl = str_lst_create(0);
	return context;
}

static void teardown(void *context) {
	str_lst_free(sl);
	sl = NULL;
}
```

### Tests
#### Is the list initially empty?

``` c
void t_empty_count(void *context) {
	assert_true(str_lst_count(sl) == 0, "empty list not empty");
}
```

#### Does the count increase by adding strings?

``` c
void t_adding_increases_count(void *context) {	
	str_lst_add(sl, "a");
	str_lst_add(sl, "b");
	assert_true(str_lst_count(sl) == 2, "wrong count");
}
```

#### Can we add a lot of strings?

To trigger the reallocation of the buffer.

``` c
void t_adding_many(void *context) {
	int count = 100;
	for (int i = count; i; --i) str_lst_add(sl, "x");
	assert_true(str_lst_count(sl) == count, "wrong count");
	assert_str(str_lst_begin(sl)[count - 1], "x");
}
```

#### Is the order of the strings correct?

``` c
void t_order(void *context) {
	str_lst_add(sl, "a");
	str_lst_add(sl, "b");
	assert_str(str_lst_begin(sl)[0], "a");
	assert_str(str_lst_begin(sl)[1], "b");
	assert_true(str_lst_begin(sl) + 2 == str_lst_end(sl));
}
```

### Fixture

``` c
int main(int argc, char **argv) {
	run_test_ex(t_empty_count, NULL, setup, teardown);
	run_test_ex(t_adding_increases_count, NULL, setup, teardown);
	run_test_ex(t_adding_many, NULL, setup, teardown);
	run_test_ex(t_order, NULL, setup, teardown);
	unit_summary();
}
```
