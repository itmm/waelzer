#include "../2.1-logging/log.h"
#include "../2.5-string-list/str_lst.h"

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

str_lst *get_list_from_content() {
	FILE *in = fopen("content.txt", "r");
	return_unless(in, NULL, "can't read content.txt");

	str_lst *result = str_lst_create(0);
	if (!result) {
		fclose(in);
		return NULL;
	}

	char buffer[256];
	while (fgets(buffer, sizeof(buffer), in)) {
		if (!str_lst_add(result, buffer)) {
			str_lst_free(result);
			fclose(in);
			return NULL;
		}
	}
	fclose(in);
	if (ferror(in)) {
		str_lst_free(result);
		return_unless(false, NULL, "reading context failed: %s\n", strerror(errno));
	}
	return result;;
}

bool has_suffix(struct dirent *entry, const char *suffix) {
	size_t length = strlen(suffix);
	return entry->d_namlen >= length && !strcmp(entry->d_name + entry->d_namlen - length, suffix);
}

int reverse_strcmp(char *a, char *b) {
	return -strcmp(a, b);
}

str_lst *get_list_from_dir() {
	DIR *dir = opendir(".");
	return_unless(dir, NULL, "can't open dir");

	str_lst *result = str_lst_create(0);
	if (!result) {
		closedir(dir);
		return NULL;
	}

	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (entry->d_namlen > 0 && entry->d_name[0] == '.') { continue; }
		if (has_suffix(entry, ".h") || has_suffix(entry, ".c") || has_suffix(entry, ".md")) {
			if (!str_lst_add(result, entry->d_name)) {
				str_lst_free(result);
				closedir(dir);
				return NULL;
			}
		}
	}

	closedir(dir);

	qsort(str_lst_begin(result), str_lst_count(result), sizeof(char *), reverse_strcmp);

	return result;
}

str_lst *get_list_of_files() {
	str_lst *result = get_list_from_content();
	if (!result) {
		result = get_list_from_dir();
	}
	return result;
}

int main(int argc, char **argv) {
	str_lst *list_of_files = get_list_of_files();
	return_unless(list_of_files, EXIT_FAILURE, "can't read list of files");
	for (char **cur = str_lst_begin(list_of_files); cur != str_lst_end(list_of_files); ++cur) {
		puts(*cur);
	}
	str_lst_free(list_of_files);
}
