#include "book_order.h"

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

bool book_order(int entries_count, char **entries) {
	if (entries_count < 0) {
		fprintf(stderr, "%s:%d entries_count %d < 0\n", __FILE__, __LINE__, entries_count);
		return false;
	} else if (entries_count == 0) {
		return true;
	} else if (!entries) {
		fprintf(stderr, "%s:%d entries not set\n", __FILE__, __LINE__);
		return false;
	}

	qsort(entries, entries_count, sizeof(char *), _comparator);

	return true;
}
