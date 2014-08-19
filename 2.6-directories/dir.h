/*>
# Reading directories

This section describes one function to read a directory in the file system into a `str_lst`. An optional filter function specifies, which entries should be added.

## Interface

First some standard includes.
<*/
#if !defined(dir_h)
#define dir_h

	#include <dirent.h>
	#include <stdbool.h>

	#include "../2.4-string-list/str_lst.h"
/*>
The filter function gets an entry, that is populated with the current directory entry.
<*/
	typedef bool (*entry_filter_fn)(struct dirent *entry);
/*>
The main function gets a path and the optional filter function as parameters and returns a new `str_lst`. The list must be freed, when it is no longer used.

If `filter` is `NULL` all entries will be returned (even the special directories `.` and `..` and files starting with a dot).  No assumptions can be made about the order of the files.
<*/
	str_lst *dir_entries(const char *path, entry_filter_fn filter);
/*>
### Helper functions for the filter function

Comparing file names in a `dirent` entry somewhat strange, because the entry contains both a string and a separate name length. The following helper functions honor the name length.

#### Is the entries name equal to `str`?
<*/
	bool dir_entry_is(struct dirent *entry, const char *str);
/*>
#### Does the entries name begin with `prefix`?
<*/
	bool dir_entry_has_prefix(struct dirent *entry, const char *prefix);
/*>
#### Does the entries name end with `suffix`?
<*/
	bool dir_entry_has_suffix(struct dirent *entry, const char *suffix);

#endif
