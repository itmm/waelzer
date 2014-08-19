# Generating the MarkDown file

In this section we will generate the MarkDown file `README.md`. The name is choosen, so it will be displayed directly in GitHub.

Basically we do three interesting things:

* get the list of files to process from the `context.txt` file, if it exists,
* get the list of files from the directory, if the `context.txt` file doesn't exist, and
* parse all the files to produce one big `README.md` file.

Before processing the markdown, let's start with the most easy stuff: building a `str_lst` of all the files that should be processed.

## What we need

As always, we need to build on prior art and don't start from
scratch. In this case we need the logging and the string list
(to keep all our files, that we want to process). The string
list needs `str.h`, but we don't use it directly here, so we
wont include it.

``` c
#include "../2.1-logging/log.h"
#include "../2.4-string-list/str_lst.h"
```

The system headers are good old friends, with the exception
of `dirent.h`. We use it to parse the directory, if `content.txt`
is not provided.

``` c
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
```
## get the list of files

We have two ways to get the files. The preferred way is to provide a `content.txt` file.
It contains a list of files. If we can't read this file, we will log an error! But to
make things easy, we will read all `*.c`, `*.h`, and `*.md` files (without `README.md`)
as a backup strategy.

The `context.txt` is clearly the preferred way, because you can provide the correct
order of the files.

The code is simple: try the `content.txt` and if it fails, load the directoy list
(which may also fail).

``` c
static str_lst *get_list_of_files() {
	str_lst *result = get_list_from_content();
	if (!result) {
		result = get_list_from_dir();
	}
	return result;
}
```
## Get list of files from `content.txt`

The first part is easy. We open the the file and create a new string list.
If one of these operations fail, we return `NULL`.

``` c
static str_lst *get_list_from_content() {
	FILE *in = fopen("content.txt", "r");
	return_unless(in, NULL, "can't read content.txt: %s", strerror(errno));

	str_lst *result = str_lst_create(0);
	if (!result) {
		fclose(in);
		return NULL;
	}
```

The reading is also straight forward. The limit of the file names should
be sufficient for all situations. As a goodie we ignore lines that are
empty or start with a `#`, so you can provide your own comments in this
file.

The `#` check is easy (you can't put spaces before the `#`!)

``` c
	static char buffer[256];
	while (fgets(buffer, sizeof(buffer), in)) {
		if (buffer[0] == '#') { continue; }
```

To see, if the line is empty, we first have to strip the newline
characters from the end of the line. That we need to do anyway, because
the file names doesn't contain this newlines.

``` c
		size_t len = strlen(buffer);
		while (len > 0 && buffer[len - 1] <= ' ') {
			--len;
			buffer[len] = 0;
		}
```

If the resulting string is empty, we can skip it.

``` c
		if (!*buffer) { continue; }
```

If the adding of the file name fails, we have to free the memory of the
list and closing the file before returning.

If we can't add any entry, we will return `NULL`, not the partial created
list. The only way this should fail, if you run out of memory. Then you
probably have the wrong format anyway.

``` c
		if (!str_lst_add(result, buffer)) {
			str_lst_free(result);
			fclose(in);
			return NULL;
		}
	}
```

If we get an error, we will also reject all entries readed so far. In
this case, we will log an error, because the methods called will not
have done so.

``` c
	if (ferror(in)) {
		str_lst_free(result);
		fclose(in);
		return_unless(false, NULL, "reading context failed: %s\n", strerror(errno));
	}
	fclose(in);
	return result;;
}
```
## Reading files from the directory

In this function we open the current working directory and create a string list. If anything fails,
we return NULL.

``` c
static str_lst *get_list_from_dir() {
	DIR *dir = opendir(".");
	return_unless(dir, NULL, "can't open dir");

	str_lst *result = str_lst_create(0);
	if (!result) {
		closedir(dir);
		return NULL;
	}
```

Working with directory entries is strange, because it has an extra name field. Maybe for entries, that
contain 0 bytes in there name? If not, they can be compared using `strcmp`, but we have to check the
length also.

This will be used to ignore entries, that start with a `.`:

``` c
	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (entry->d_namlen > 0 && entry->d_name[0] == '.') { continue; }
```

Because the check for a string needs also to check its length, there is a helper method for this.
We want to ignore the `README.md` file, because that is the file we write.

``` c
		if (entry_matches(entry, "README.md")) { continue; }
```

We accept the C files (including headers) and MarkDown files (ignoring `README.md`). If the adding
of the entry fails, we bail out like in the `context.txt` parsing.

``` c
		if (has_suffix(entry, ".h") || has_suffix(entry, ".c") || has_suffix(entry, ".md")) {
			if (!str_lst_add(result, entry->d_name)) {
				str_lst_free(result);
				closedir(dir);
				return NULL;
			}
		}
	}

	closedir(dir);
```

We sorting the list inversly, so that if all file types are present for a base name, the order will be

1. `bla.md`
2. `bla.h`
3. `bla.c`

Also we guarantee, that the order will always be the same and does not depend on file system magic.

``` c
	qsort(str_lst_begin(result), str_lst_count(result), sizeof(char *), reverse_strcmp);

	return result;
}
```
To check for a match in a direntry, we have to check the length first.

``` c
static bool entry_matches(struct dirent *entry, const char *name) {
	size_t length = strlen(name);
	return entry->d_namlen == length && !strcmp(entry->d_name, name);
}
```

The suffix check contains a grain of magic. We can use good ol' `strcmp`
to peform the check, if we know where to start it. We need to shift the
candidate by its length minus the suffix length.

Of course this only makes sense, if the suffix length is smaller.

``` c
static bool has_suffix(struct dirent *entry, const char *suffix) {
	size_t length = strlen(suffix);
	return entry->d_namlen >= length && !strcmp(entry->d_name + entry->d_namlen - length, suffix);
}
```

The reverse comparator is simply the reverse of `strcmp`. We could also
exchange the parameters, but I prefer the negation.

``` c
static int reverse_strcmp(const void*a, const void *b) {
	return -strcmp(a, b);
}
```
## Processing the input files

So now we can start to process the files. Let's jump to the `main` function
and see the good parts. First we get files to process and open the output
file.

If there are no input files, we terminate and wont overwrite an existing
`README.md` file (just be save).

``` c
int main(int argc, char **argv) {
	str_lst *files = get_list_of_files();
	return_unless(files, EXIT_FAILURE, "can't read list of files");
	return_unless(str_lst_count(files), EXIT_SUCCESS, "nothing to process");

	FILE *out = fopen("README.md", "w");
	if (!out) {
		str_lst_free(files);
		return_unless(false, EXIT_FAILURE, "can't open README.md");
	}
```

Now we open each file and call the process function. The function needs to know,
if the file contains MarkDown directly, or is a source code file, that only
contains MarkDown in special comment blocks.

We do the suffix magic to check, if it ends in `.md`. Otherwise we will assume
it a source code file.

``` c
	for (char **cur = str_lst_begin(files); cur != str_lst_end(files); ++cur) {
		FILE *in = fopen(*cur, "r");
		if (!in) {
			fclose(out);
			return_unless(false, EXIT_FAILURE, "can't read \"%s\"", *cur);
		}

		printf("processing \"%s\" ... ", *cur);
		size_t length = strlen(*cur);
		bool code_file = length < 3 || strcmp(*cur + length - 3, ".md");
		puts(process_file(in, out, code_file) ? "ok" : "FAILED");
		fclose(in);
	}
	fclose(out);
	str_lst_free(files);
}
```
In the process file method we keep two bits of state:

* is the next line we read code or MarkDown?
* is the next line we write code or MarkDown?

The kind of the file sets the first flag, and we always start writing in
MarkDown mode.

``` c
static bool process_file(FILE *in, FILE *out, bool code_file) {
	bool read_in_code = code_file;
	bool write_in_code = false;
```

The reading looks like the reading of `context.txt`, but the lines can
be much longer. Some MarkDown tools create a line per paragraph. Maybe
we need a better method, but for a start 10k seems enough.

``` c
	static char buffer[10240]; // long lines for .md files
	while (fgets(buffer, sizeof(buffer), in)) {
```

If a code line starts with the special comment, we start reading
MarkDown that is inside the comment.

``` c
		if (code_file && strstr(buffer, "/*>") == buffer) {
			read_in_code = false;
```

If we are reading MarkDown in code and a line starts with the special
ending comment, we stop reading MarkDown and switch back to code.

``` c
		} else if (code_file && strstr(buffer, "<*/") == buffer) {
			read_in_code = true;
		} else {
```

If we have read a line of code, but are in MarkDown writing, we send a
MarkDown command to switch to code.

``` c
			if (read_in_code && !write_in_code) {
				fprintf(out, "\n``` c\n");
				write_in_code = true;
```

Likewise we switch back to MarkDown, if we are still in writing code.

``` c
			} else if (!read_in_code && write_in_code) {
				fprintf(out, "```\n\n");
				write_in_code = false;
			}
			fprintf(out, "%s", buffer);
		}
	}
```

If we ended the file in code writing mode, we need to switch back.

``` c
	if (write_in_code) {
		fprintf(out, "```\n");
	}
	return true;
}
```
That was this section. I hope you enjoyed it.

All that is left, is the main file, that fetches all the fragments.

``` c
#include "what_we_need.c"
#include "get_list_from_content.c"
#include "get_files_from_dir_helper.c"
#include "get_files_from_dir.c"
#include "get_list_of_files.c"
#include "process_file.c"
#include "main.c"
```
