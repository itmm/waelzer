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
<*/
str_lst *dir_entries(const char *path, entry_filter_fn filter) {
	DIR *dir = opendir(path);
	return_unless(dir, NULL, "can't open dir: %s", strerror(errno));

	str_lst *result = str_lst_create(0);
	if (!result) {
		closedir(dir);
		return NULL;
	}
/*>
Now we iterate over the entries and add them to the result list, if the filter does not reject them.
<*/
	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (!filter || filter(entry)) {
			if (!str_lst_add(result, entry->d_name)) {
				str_lst_free(result);
				closedir(dir);
				return NULL;
			}
		}
	}

	closedir(dir);
	return result;
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

