# Copy Source files

In this section, all source files from all sections are copied into a build directory. There they can be build and linked without using the relative paths in the include statements. That will ease the programming a lot.

## Structure of the Program

The program contains of the following steps:

1. Create and empty a `build` directory. 
2. Process each section directory and copy its source files to the `build` directory.

We assume a UNIX-like environment, so we can shortcut the steps of creating and emptying the `build` directory by issuing two `system` calls.

``` c
int main(int argc, char **argv) {
	return_unless(!system("mkdir -p build"), EXIT_FAILURE, "can't create dir");
	return_unless(!system("rm -Rf build/*"), EXIT_FAILURE, "can't clear build dir");
	str_lst *directories = dir_entries(".", sections_filter);
	bool result = process_directories(directories);
	str_lst_free(directories);
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
```
## Reading the section directories

First we need a filter that returns only section directories. We return all entries, that start with a digit.

``` c
static bool sections_filter (struct dirent *entry) {
	return entry->d_namlen > 0 && isdigit(entry->d_name[0]);
}
```

The resulting directories are ordered correctly and then processed one by one.

Due to the order, the earlier sections are copied first and may be replaced by files from later sections.

``` c
static bool process_directories(str_lst *directories) {
	return_unless(directories, false);
	return_unless(book_order(directories), false);

	for (char **cur = str_lst_begin(directories); cur != str_lst_end(directories); ++cur) {
		return_unless(process_directory(*cur), false);
	}
	return true;
}
```
For each directory, we read its source files and call a function to process these files.

``` c
static bool process_directory(char *name) {
	printf("processing %s ... ", name);
	str_lst *entries = dir_entries(name, source_filter);
	bool result = process_entries(entries, name);
	str_lst_free(entries);
	puts(result ? "ok" : "FAILED");
	return result;
}
```
## Wrapping Up

For the final program we need bunch of includes and also include our parts in the right order.

``` c
#include "../2.1-logging/log.h"
#include "../2.3-strings/str.h"
#include "../2.4-string-list/str_lst.h"
#include "../2.6-directories/dir.h"
#include "../4.1-book-order/book_order.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "execute_buffer.c"
#include "process_entries.c"
#include "process_directory.c"
#include "read_dirs.c"
#include "main.c"
```
