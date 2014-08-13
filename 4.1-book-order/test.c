#include "book_order.h"

#include "../2.2-unit-tests/unit.h"
#include "../2.4-string-list/str_lst.h"

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

void t_simple(void *context) {
	char *test_case[] = { "1.1-d", "3-ab", "2-x" };
	char *expected[] = { "3-ab", "2-x", "1.1-d" };
	assert_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}

void t_big_nums(void *context) {
	char *test_case[] = { "2-b", "10-a" };
	char *expected[] = { "10-a", "2-b" };
	assert_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}

void t_inner_counting(void *context) {
	char *test_case[] = { "1.2", "1.10" };
	char *expected[] = { "1.10", "1.2" };
	assert_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}

void t_empty(void *context) {
	assert_test(0, NULL, NULL);
}

void t_null_failure(void *context) {
	log_adapter_fn original = disable_logging();
	assert_true(!book_order(NULL), "book_order should fail");
	set_log_adapter(original);
}

int main(int argc, char **argv) {
	run_test(t_simple, NULL);
	run_test(t_big_nums, NULL);
	run_test(t_inner_counting, NULL);
	run_test(t_empty, NULL);
	run_test(t_null_failure, NULL);
	unit_summary();
}
