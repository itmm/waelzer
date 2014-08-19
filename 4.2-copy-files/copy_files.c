#include "../2.1-logging/log.h"
#include "../2.4-string-list/str_lst.h"
#include "../2.6-directories/dir.h"
#include "../4.1-book-order/book_order.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

bool sections_filter (struct dirent *entry) {
	return entry->d_namlen > 0 && isdigit(entry->d_name[0]);
}

void process_directory(char *name) {
	puts(name);
}

int main(int argc, char **argv) {
	str_lst *directories = dir_entries(".", sections_filter);
	return_unless(directories, EXIT_FAILURE);
	return_unless(book_order(directories), EXIT_FAILURE);

	for (char **cur = str_lst_begin(directories); cur != str_lst_end(directories); ++cur) {
		process_directory(*cur);
	}
	str_free(directories);
}
