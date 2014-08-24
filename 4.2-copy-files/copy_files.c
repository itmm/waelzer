#include "../2.1-logging/log.h"
#include "../2.4-string-list/str_lst.h"
#include "../2.6-directories/dir.h"
#include "../4.1-book-order/book_order.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool sections_filter (struct dirent *entry) {
	return entry->d_namlen > 0 && isdigit(entry->d_name[0]);
}

bool source_filter (struct dirent *entry) {
	return dir_entry_has_suffix(entry, ".c") || dir_entry_has_suffix(entry, ".h");
}

bool process_directory(char *name) {
	printf("processing %s\n", name);
	str_lst *entries = dir_entries(name, source_filter);
	return_unless(entries, false);
	for (char **cur = str_lst_begin(entries); cur != str_lst_end(entries); ++cur) {
		char *buffer = NULL;
		return_unless(
			asprintf(&buffer, "cp '%s/%s' build/", name, *cur) >= 0, 
			false,
			"file path too long: '%s/%s'", name, *cur
		);
		return_unless(!system(buffer), false, "can't copy '%s/%s'", name, *cur);
		free(buffer);
	}
	str_lst_free(entries);
	return true;
}

int main(int argc, char **argv) {
	return_unless(!system("mkdir -p build"), EXIT_FAILURE, "can't create dir");
	return_unless(!system("rm -Rf build/*"), EXIT_FAILURE, "can't clear build dir");
	str_lst *directories = dir_entries(".", sections_filter);
	return_unless(directories, EXIT_FAILURE);
	return_unless(book_order(directories), EXIT_FAILURE);

	for (char **cur = str_lst_begin(directories); cur != str_lst_end(directories); ++cur) {
		process_directory(*cur);
	}
	str_lst_free(directories);
}
