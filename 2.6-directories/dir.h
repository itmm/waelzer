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
