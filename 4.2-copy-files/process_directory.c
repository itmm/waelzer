/*>
For each directory, we read its source files and call a function to process these files.
<*/
static bool process_directory(char *name) {
	printf("processing %s ... ", name);
	str_lst *entries = dir_entries(name, source_filter);
	bool result = process_entries(entries, name);
	str_lst_free(entries);
	puts(result ? "ok" : "FAILED");
	return result;
}
