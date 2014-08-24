/*>
## Implementation

First we need a couple of include files.
<*/
#include "dir.h"

#include "../2.1-logging/log.h"

#include <errno.h>
#include <string.h>
/*>
### Reading the entries

The function opens the directory and creates the return list.

There are three parts, that can fail:

1. opening the directory,
2. creating the result `str_lst`,
3. adding entries to the result list.

Therefore this function is split into three functions. The first opens and closes the directory (even, if the rest fails).
<*/
static str_lst *read_dir(DIR *dir, entry_filter_fn filter);

str_lst *dir_entries(const char *path, entry_filter_fn filter) {
	DIR *dir = opendir(path);
	return_unless(dir, NULL, "can't open dir: %s", strerror(errno));

	str_lst *result = read_dir(dir, filter);

	closedir(dir);
	return result;
}
/*>
The second function creates the result list. If something goes wrong while reading the entries, the list will be freed before returning.
<*/
static bool read_dir_entries(DIR *dir, str_lst *result, entry_filter_fn filter);

static str_lst *read_dir(DIR *dir, entry_filter_fn filter) {
	return_unless(dir, NULL, "no directory");
	str_lst *result = str_lst_create(0);
	if (!read_dir_entries(dir, result, filter)) {
		result = str_lst_free(result);
	}
	return result;
}
/*>
The last function iterates over the entries.
<*/
static bool read_dir_entries(DIR *dir, str_lst *result, entry_filter_fn filter) {
	return_unless(dir, false, "no directory");
	return_unless(result, false, "no result list");
/*>
The entries will be added to the result list, if the filter does not reject them.
<*/
	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (!filter || filter(entry)) {
			return_unless(str_lst_add(result, entry->d_name), false, "can't add entry");
		}
	}
	return true;
}
/*>
### Helper functions

#### Compare the entry name

We need to compare the string and the name length entries.
<*/
bool dir_entry_is(struct dirent *entry, const char *str) {
	return_unless(entry, false, "entry is NULL");
	return_unless(str, false, "str is NULL");
	return entry->d_namlen == strlen(str) && !strcmp(entry->d_name, str);
}
/*>
#### Compare a prefix

A string is a prefix, if it is a substring starting at index position 0.
<*/
bool dir_entry_has_prefix(struct dirent *entry, const char *prefix) {
	return_unless(entry, false, "entry is NULL");
	return_unless(prefix, false, "prefix is NULL");
	return entry->d_namlen >= strlen(prefix) && strstr(entry->d_name, prefix) == entry->d_name;
}
/*>
#### Compare a suffix

A string is a suffix, if it last bytes of the string are equal to the suffix.
<*/
bool dir_entry_has_suffix(struct dirent *entry, const char *suffix) {
	return_unless(entry, false, "entry is NULL");
	return_unless(suffix, false, "suffix is NULL");
	size_t len = strlen(suffix);
	return entry->d_namlen >= len && !strcmp(entry->d_name + entry->d_namlen - len, suffix);
}

