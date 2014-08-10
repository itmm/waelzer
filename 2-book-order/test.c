#include "book_order.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_test(int count, char *test_case[], char *expected[]) {
	if (!book_order(count, test_case)) {
		fprintf(stderr, "book_order failed\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < count; ++i) {
		if (strcmp(test_case[i], expected[i])) {
			fprintf(stderr, "entry %d does not match '%s' != '%s'\n", i, test_case[i], expected[i]);
			exit(EXIT_FAILURE);
		}
	}
	putchar('.');
}

void t_simple() {
	char *test_case[] = { "1.1-d", "3-ab", "2-x" };
	char *expected[] = { "3-ab", "2-x", "1.1-d" };
	run_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}

void t_big_nums() {
	char *test_case[] = { "2-b", "10-a" };
	char *expected[] = { "10-a", "2-b" };
	run_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}

void t_inner_counting() {
	char *test_case[] = { "1.2", "1.10" };
	char *expected[] = { "1.10", "1.2" };
	run_test(sizeof(test_case)/sizeof(char *), test_case, expected);
}

void t_empty() {
	run_test(0, NULL, NULL);
}

int main(int argc, char **argv) {
	t_simple();
	t_big_nums();
	t_inner_counting();
	t_empty();
	puts("\nok");
}
