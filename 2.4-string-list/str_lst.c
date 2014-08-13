#include "str_lst.h"

#include "../2.1-logging/log.h"
#include "../2.3-strings/str.h"

#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct str_lst {
	char **begin;
	char **end;
	char **capacity;
};

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

void str_lst_free(str_lst *sl) {
	if (!sl) { return; }
	if (sl->begin) {
		for (char **i = sl->begin; i != sl->end; ++i) { str_free(*i); }
		free(sl->begin);
	}
	free(sl);
}

int str_lst_count(str_lst *sl) {
	if (!sl) { return 0; }
	return sl->end - sl->begin;
}

char **str_lst_begin(str_lst *sl) {
	return sl ? sl->begin : NULL;
}

char **str_lst_end(str_lst *sl) {
	return sl ? sl->end : NULL;
}

bool str_lst_add(str_lst *sl, const char *str) {
	return str_lst_add_and_consume(sl, str_cons(1, str));
}

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

