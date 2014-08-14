# Find the Section Order

All sections start with a sequence of numbers separated by dots followed by
a hyphen. The order should compare by number, not by digit, e.g.
`2.6` < `10.2` < `10.3`.

## Interface

The function `book_order` sorts a string list according to this order.
``` c
#if !defined(book_order_h)
#define book_order_h

	#if !defined(str_lst_h)
		#include "../2.4-string-list/str_lst.h"
	#endif

	#include <stdbool.h>

	bool book_order(str_lst *strs);

#endif
```
## Implementation

First we need a couple of imports.
``` c
#include "book_order.h"

#include "../2.1-logging/log.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```
### Comparator

We need a special comparator to compare two strings.
``` c
static int _comparator(const void *a, const void *b) {
	const char *as = *(const char **) a;
	const char *bs = *(const char **) b;
```
While one of the strings has a digit at its current
position, we read two numbers from the two strings and
compare them (if one string doesn't start with a digit,
its number is zero.
``` c
	while (isdigit(*as) || isdigit(*bs)) {
		int an = 0;
		int bn = 0;

		while (isdigit(*as)) { an = an * 10 + *as++; }
		while (isdigit(*bs)) { bn = bn * 10 + *bs++; }

```
If the numbers are not equal, the comparison is done
``` c
		if (an < bn) { return -1; }
		else if (an > bn) { return 1; }
```
Otherwise eat dots and loop.
``` c
		if (*as == '.') { ++as; }
		if (*bs == '.') { ++bs; }
	}
```
The rest of the strings are compared also.
``` c
	return strcmp(as, bs);
}
```
### Ordering the strings

The ordering is a simple call to quicksort with our new comparator.
``` c
bool book_order(str_lst *strs) {
	return_unless(strs, false, "no string list");
	int count = str_lst_count(strs);
	if (count == 0) { return true; }

	qsort(str_lst_begin(strs), count, sizeof(char *), _comparator);

	return true;
}
```
## Unit-Tests

First some imports.
``` c
#include "book_order.h"

#include "../2.2-unit-tests/unit.h"
#include "../2.4-string-list/str_lst.h"
```
### Assert function

This function asserts, if the test_case is equal to expected, if sorted.
``` c
void assert_test(int count, char *test_case[], char *expected[]) {
	str_lst *lst = str_lst_create(count);
	char **cur = test_case;
	for (int i = count; i; --i) { str_lst_add(lst, *cur++); }

	assert_true(book_order(lst), "book_order failed");

	cur = str_lst_begin(lst);
	char **exp = expected;
	for (int i = count; i; --i) {
		assert_str(*cur++, *exp++);
	}
}
```
### Tests
#### Test order with some single digit numbers
``` c
void t_simple(void *context) {
	char *test_case[] = { "1.1-d", "3-ab", "2-x" };
	char *expected[] = { "1.1-d", "2-x", "3-ab" };
	assert_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}
```
#### Test order with two digit number
``` c
void t_big_nums(void *context) {
	char *test_case[] = { "10-a", "2-b" };
	char *expected[] = { "2-b", "10-a" };
	assert_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}
```
#### Test order on second number
``` c
void t_inner_counting(void *context) {
	char *test_case[] = { "1.10", "1.2" };
	char *expected[] = { "1.2", "1.10" };
	assert_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}
```
#### Test sorting of empty list
``` c
void t_empty(void *context) {
	assert_test(0, NULL, NULL);
}
```
#### Test call with `NULL`
``` c
void t_null_failure(void *context) {
	log_adapter_fn original = disable_logging();
	assert_true(!book_order(NULL), "book_order should fail");
	set_log_adapter(original);
}
```
### Fixture
``` c
int main(int argc, char **argv) {
	run_test(t_simple, NULL);
	run_test(t_big_nums, NULL);
	run_test(t_inner_counting, NULL);
	run_test(t_empty, NULL);
	run_test(t_null_failure, NULL);
	unit_summary();
}
```
