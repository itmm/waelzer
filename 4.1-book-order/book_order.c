/*>
## Implementation

First we need a couple of imports.
<*/
#include "book_order.h"

#include "../2.1-logging/log.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*>
### Comparator

We need a special comparator to compare two strings.
<*/
static int _comparator(const void *a, const void *b) {
	const char *as = *(const char **) a;
	const char *bs = *(const char **) b;
/*>
While one of the strings has a digit at its current
position, we read two numbers from the two strings and
compare them (if one string doesn't start with a digit,
its number is zero.
<*/
	while (isdigit(*as) || isdigit(*bs)) {
		int an = 0;
		int bn = 0;

		while (isdigit(*as)) { an = an * 10 + *as++; }
		while (isdigit(*bs)) { bn = bn * 10 + *bs++; }

/*>
If the numbers are not equal, the comparison is done
<*/
		if (an > bn) { return -1; }
		else if (an < bn) { return 1; }
/*>
Otherwise eat dots and loop.
<*/
		if (*as == '.') { ++as; }
		if (*bs == '.') { ++bs; }
	}
/*>
The rest of the strings are compared in reverse also.
<*/
	return -strcmp(as, bs);
}
/*>
### Ordering the strings

The ordering is a simple call to quicksort with our new comparator.
<*/
bool book_order(str_lst *strs) {
	return_unless(strs, false, "no string list");
	int count = str_lst_count(strs);
	if (count == 0) { return true; }

	qsort(str_lst_begin(strs), count, sizeof(char *), _comparator);

	return true;
}
