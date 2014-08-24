/*>
## Reading the section directories

First we need a filter that returns only section directories. We return all entries, that start with a digit.
<*/
static bool sections_filter (struct dirent *entry) {
	return entry->d_namlen > 0 && isdigit(entry->d_name[0]);
}
/*>
The resulting directories are ordered correctly and then processed one by one.

Due to the order, the earlier sections are copied first and may be replaced by files from later sections.
<*/
static bool process_directories(str_lst *directories) {
	return_unless(directories, false);
	return_unless(book_order(directories), false);

	for (char **cur = str_lst_begin(directories); cur != str_lst_end(directories); ++cur) {
		return_unless(process_directory(*cur), false);
	}
	return true;
}
