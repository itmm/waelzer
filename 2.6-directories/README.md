``` c
#if !defined(dir_h)
#define dir_h

	#include <dirent.h>
	#include <stdbool.h>

	#include "../2.4-string-list/str_lst.h"

	typedef bool (*entry_filter_fn)(struct dirent *entry);

	str_lst dir_entries(const char *path, entry_filter_fn filter);

	bool dir_entry_is(struct dirent *entry, const char *str);
	bool dir_entry_has_prefix(struct dirent *entry, const char *prefix);
	bool dir_entry_has_suffix(struct dirent *entry, const char *suffix);

#endif
```
``` c
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
