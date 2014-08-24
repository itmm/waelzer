# Reading directories

This section describes one function to read a directory in the file system into a `str_lst`. An optional filter function specifies, which entries should be added.

## Interface

First some standard includes.

``` c
#if !defined(dir_h)
#define dir_h

	#include <dirent.h>
	#include <stdbool.h>

	#include "../2.4-string-list/str_lst.h"
```

The filter function gets an entry, that is populated with the current directory entry.

``` c
	typedef bool (*entry_filter_fn)(struct dirent *entry);
```

The main function gets a path and the optional filter function as parameters and returns a new `str_lst`. The list must be freed, when it is no longer used.

If `filter` is `NULL` all entries will be returned (even the special directories `.` and `..` and files starting with a dot).  No assumptions can be made about the order of the files.

``` c
	str_lst *dir_entries(const char *path, entry_filter_fn filter);
```

### Helper functions for the filter function

Comparing file names in a `dirent` entry somewhat strange, because the entry contains both a string and a separate name length. The following helper functions honor the name length.

#### Is the entries name equal to `str`?

``` c
	bool dir_entry_is(struct dirent *entry, const char *str);
```

#### Does the entries name begin with `prefix`?

``` c
	bool dir_entry_has_prefix(struct dirent *entry, const char *prefix);
```

#### Does the entries name end with `suffix`?

``` c
	bool dir_entry_has_suffix(struct dirent *entry, const char *suffix);

#endif
```
## Implementation

First we need a couple of include files.

``` c
#include "dir.h"

#include "../2.1-logging/log.h"

#include <errno.h>
#include <string.h>
```

### Reading the entries

The function opens the directory and creates the return list.

There are three parts, that can fail:

1. opening the directory,
2. creating the result `str_lst`,
3. adding entries to the result list.

Therefore this function is split into three functions. The first opens and closes the directory (even, if the rest fails).

``` c
static str_lst *read_dir(DIR *dir, entry_filter_fn filter);

str_lst *dir_entries(const char *path, entry_filter_fn filter) {
	DIR *dir = opendir(path);
	return_unless(dir, NULL, "can't open dir: %s", strerror(errno));

	str_lst *result = read_dir(dir, filter);

	closedir(dir);
	return result;
}
```

The second function creates the result list. If something goes wrong while reading the entries, the list will be freed before returning.

``` c
static bool read_dir_entries(DIR *dir, str_lst *result, entry_filter_fn filter);

static str_lst *read_dir(DIR *dir, entry_filter_fn filter) {
	return_unless(dir, NULL, "no directory");
	str_lst *result = str_lst_create(0);
	if (!read_dir_entries(dir, result, filter)) {
		result = str_lst_free(result);
	}
	return result;
}
```

The last function iterates over the entries.

``` c
static bool read_dir_entries(DIR *dir, str_lst *result, entry_filter_fn filter) {
	return_unless(dir, false, "no directory");
	return_unless(result, false, "no result list");
```

The entries will be added to the result list, if the filter does not reject them.

``` c
	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (!filter || filter(entry)) {
			return_unless(str_lst_add(result, entry->d_name), false, "can't add entry");
		}
	}
	return true;
}
```

### Helper functions

#### Compare the entry name

We need to compare the string and the name length entries.

``` c
bool dir_entry_is(struct dirent *entry, const char *str) {
	return_unless(entry, false, "entry is NULL");
	return_unless(str, false, "str is NULL");
	return entry->d_namlen == strlen(str) && !strcmp(entry->d_name, str);
}
```

#### Compare a prefix

A string is a prefix, if it is a substring starting at index position 0.

``` c
bool dir_entry_has_prefix(struct dirent *entry, const char *prefix) {
	return_unless(entry, false, "entry is NULL");
	return_unless(prefix, false, "prefix is NULL");
	return entry->d_namlen >= strlen(prefix) && strstr(entry->d_name, prefix) == entry->d_name;
}
```

#### Compare a suffix

A string is a suffix, if it last bytes of the string are equal to the suffix.

``` c
bool dir_entry_has_suffix(struct dirent *entry, const char *suffix) {
	return_unless(entry, false, "entry is NULL");
	return_unless(suffix, false, "suffix is NULL");
	size_t len = strlen(suffix);
	return entry->d_namlen >= len && !strcmp(entry->d_name + entry->d_namlen - len, suffix);
}

```
## Unit-Tests

First we have some headers

``` c
#include "dir.h"

#include "../2.2-unit-tests/unit.h"
```

### Tests
#### Simple entry comparison

``` c
void t_entry_is_equal(void *context) {
	struct dirent entry = { .d_namlen = 3, .d_name = "abc" };
	assert_true(dir_entry_is(&entry, "abc"));
}
```

#### Simple prefix comparison

``` c
void t_entry_prefix(void *context) {
	struct dirent entry = { .d_namlen = 3, .d_name = "abc" };
	assert_true(dir_entry_has_prefix(&entry, "ab"));
}
```

#### Simple suffix comparison

``` c
void t_entry_suffix(void *context) {
	struct dirent entry = { .d_namlen = 3, .d_name = "abc" };
	assert_true(dir_entry_has_suffix(&entry, "bc"));
}

```

### Fixture

``` c
int main(int argc, char **argv) {
	run_test(t_entry_is_equal, NULL);
	run_test(t_entry_prefix, NULL);
	run_test(t_entry_suffix, NULL);
	unit_summary();
}
```
