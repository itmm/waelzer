#include "log.h"
#include "t_logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static log_fn original_logger;

void assert_messages(const char *expected) {
	char *messages = t_logger_copy_messages();
	if (strcmp(messages, expected)) {
		fprintf(stderr, "%s:%d different: '%s' != '%s'\n", __FILE__, __LINE__, messages, expected);
		free(messages);
		exit(EXIT_FAILURE);
	}
	free(messages);
	putchar('.');
}

void t_simple() {
	log("test %s", "abc");
	assert_messages("test abc");
}

void t_empty() {
	log();
	assert_messages("");
}

int main(int argc, char **argv) {
	original_logger = set_logger(t_logger_fn);
	t_simple();
	t_logger_clear_messages();
	t_empty();
	set_logger(original_logger);
	t_logger_clear_messages();
	puts("\nok");
}
