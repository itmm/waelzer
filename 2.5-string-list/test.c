#include "str_lst.h"

#include "../2.2-unit-tests/unit.h"
#include "../2.3-strings/str.h"

#include <string.h>

static str_lst *sl;

static void *setup(void *context) {
	sl = str_lst_create(0);
	return context;
}

static void teardown(void *context) {
	str_lst_free(sl);
	sl = NULL;
}

void t_empty_count(void *context) {
	assert_true(str_lst_count(sl) == 0, "empty list not empty");
}

void t_adding_increases_count(void *context) {	
	str_lst_add(sl, "a");
	str_lst_add(sl, "b");
	assert_true(str_lst_count(sl) == 2, "wrong count");
}

void t_adding_many(void *context) {
	int count = 100;
	for (int i = count; i; --i) str_lst_add(sl, "x");
	assert_true(str_lst_count(sl) == count, "wrong count");
	assert_str(str_lst_begin(sl)[count - 1], "x");
}

void t_order(void *context) {
	str_lst_add(sl, "a");
	str_lst_add(sl, "b");
	assert_str(str_lst_begin(sl)[0], "a");
	assert_str(str_lst_begin(sl)[1], "b");
	assert_true(str_lst_begin(sl) + 2 == str_lst_end(sl));
}

int main(int argc, char **argv) {
	run_test_ex(t_empty_count, NULL, setup, teardown);
	run_test_ex(t_adding_increases_count, NULL, setup, teardown);
	run_test_ex(t_adding_many, NULL, setup, teardown);
	run_test_ex(t_order, NULL, setup, teardown);
	unit_summary();
}
