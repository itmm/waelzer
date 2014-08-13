#include "book_order.h"

#include "../2.1-logging/log.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _comparator(const void *a, const void *b) {
	const char *as = *(const char **) a;
	const char *bs = *(const char **) b;

	while (isdigit(*as) || isdigit(*bs)) {
		int an = 0;
		int bn = 0;

		while (isdigit(*as)) { an = an * 10 + *as++; }
		while (isdigit(*bs)) { bn = bn * 10 + *bs++; }

		if (*as == '.') { ++as; }
		if (*bs == '.') { ++bs; }

		if (an > bn) { return -1; }
		else if (an < bn) { return 1; }
	}
	return -strcmp(as, bs);
}

bool book_order(str_lst *strs) {
	return_unless(strs, false, "no string list");
	int count = str_lst_count(strs);
	if (count == 0) { return true; }

	qsort(str_lst_begin(strs), count, sizeof(char *), _comparator);

	return true;
}
