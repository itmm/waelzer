/*>
## Reading files from the directory

In this function we open the current working directory and create a string list. If anything fails, we return NULL.
<*/
static str_lst *get_list_from_dir() {
	DIR *dir = opendir(".");
	return_unless(dir, NULL, "can't open dir");

	str_lst *result = str_lst_create(0);
	if (!result) {
		closedir(dir);
		return NULL;
	}
/*>
Working with directory entries is strange, because it has an extra name field. Maybe for entries, that contain 0 bytes in their name? If not, they can be compared using `strcmp`, but we have to check the length also.

This will be used to ignore entries, that start with a `.`:
<*/
	struct dirent *entry;
	while ((entry = readdir(dir))) {
		if (entry->d_namlen > 0 && entry->d_name[0] == '.') { continue; }
/*>
Because the check for a string needs also to check its length, there is a helper method for this.  We want to ignore the `README.md` file, because that is the file we write.
<*/
		if (entry_matches(entry, "README.md")) { continue; }
/*>
We accept the C files (including headers) and MarkDown files (ignoring `README.md`). If the adding of the entry fails, we bail out like in the `context.txt` parsing.
<*/
		if (has_suffix(entry, ".h") || has_suffix(entry, ".c") || has_suffix(entry, ".md")) {
			if (!str_lst_add(result, entry->d_name)) {
				str_lst_free(result);
				closedir(dir);
				return NULL;
			}
		}
	}

	closedir(dir);
/*>
We sorting the list inversely, so that if all file types are present for a base name, the order will be

1. `bla.md`
2. `bla.h`
3. `bla.c`

Also we guarantee, that the order will always be the same and does not depend on file system magic.
<*/
	qsort(str_lst_begin(result), str_lst_count(result), sizeof(char *), reverse_strcmp);

	return result;
}
