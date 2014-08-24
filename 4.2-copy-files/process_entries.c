/*>
## Processing entries in section directory

First we have a filter for all source files. Currently we only support C files.
<*/
static bool source_filter (struct dirent *entry) {
	return dir_entry_has_suffix(entry, ".c") || dir_entry_has_suffix(entry, ".h");
}

/*>
To process the files, we stack together a copy command and try to execute it in the function `execute_buffer`. If the execution fails, we return here and can free the allocated buffer.
<*/
static bool process_entries(str_lst *entries, const char *dir) {
	return_unless(entries, false);
	for (char **cur = str_lst_begin(entries); cur != str_lst_end(entries); ++cur) {
		char *buffer = NULL;
		return_unless(
			asprintf(&buffer, "cp '%s/%s' build/", dir, *cur) >= 0, 
			false,
			"can't allocate command"
		);
		bool result = execute_buffer(buffer);
		str_free(buffer);
		if (!result) { return false; }
	}
	return true;
}
/*>
If you feel, that you can't use `return_unless` without creating a memory leak, often you can avoid the leak by splitting up the function. It makes the code clearer.
<*/
