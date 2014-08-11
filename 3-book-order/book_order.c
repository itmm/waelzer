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

bool book_order(int entries_count, char **entries) {
	return_unless(entries_count >= 0, false, "entries count %d < 0", entries_count);
	if (entries_count == 0) { return true; }
	return_unless(entries, false, "entries not set");

	qsort(entries, entries_count, sizeof(char *), _comparator);

	return true;
}
