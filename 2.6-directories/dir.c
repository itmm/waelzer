#include "dir.h"

#include "../2.1-logging/log.h"

#include <string.h>

bool dir_entry_is(struct dirent *entry, const char *str) {
	return_unless(entry, false, "entry is NULL");
	return_unless(str, false, "str is NULL");
	return entry->d_namlen == strlen(str) && !strcmp(entry->d_name, str);
}

bool dir_entry_has_prefix(struct dirent *entry, const char *prefix) {
	return_unless(entry, false, "entry is NULL");
	return_unless(prefix, false, "prefix is NULL");
	return entry->d_namlen >= strlen(prefix) && strstr(entry->d_name, prefix) == entry->d_name;
}

bool dir_entry_has_suffix(struct dirent *entry, const char *suffix) {
	return_unless(entry, false, "entry is NULL");
	return_unless(suffix, false, "suffix is NULL");
	size_t len = strlen(suffix);
	return entry->d_namlen >= len && !strcmp(entry->d_name + entry->d_namlen - len, suffix);
}

