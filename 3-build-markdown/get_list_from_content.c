/*>
## Get list of files from `content.txt`

The first part is easy. We open the file and create a new string list.  If one of these operations fail, we return `NULL`.
<*/
static str_lst *get_list_from_content() {
	FILE *in = fopen("content.txt", "r");
	return_unless(in, NULL, "can't read content.txt: %s", strerror(errno));

	str_lst *result = str_lst_create(0);
	if (!result) {
		fclose(in);
		return NULL;
	}
/*>
The reading is also straight forward. The limit of the file names should be sufficient for all situations. As a goodie we ignore lines that are empty or start with a `#`, so you can provide your own comments in this file.

The `#` check is easy (you can't put spaces before the `#`!)
<*/
	static char buffer[256];
	while (fgets(buffer, sizeof(buffer), in)) {
		if (buffer[0] == '#') { continue; }
/*>
To see, if the line is empty, we first have to strip the newline characters from the end of the line. That we need to do anyway, because the file names doesn't contain this newlines.
<*/
		size_t len = strlen(buffer);
		while (len > 0 && buffer[len - 1] <= ' ') {
			--len;
			buffer[len] = 0;
		}
/*>
If the resulting string is empty, we can skip it.
<*/
		if (!*buffer) { continue; }
/*>
If the adding of the file name fails, we have to free the memory of the list and closing the file before returning.

If we can't add any entry, we will return `NULL`, not the partial created list. The only way this should fail, if you run out of memory. Then you probably have the wrong format anyway.
<*/
		if (!str_lst_add(result, buffer)) {
			str_lst_free(result);
			fclose(in);
			return NULL;
		}
	}
/*>
If we get an error, we will also reject all entries read so far. In this case, we will log an error, because the methods called will not have done so.
<*/
	if (ferror(in)) {
		str_lst_free(result);
		fclose(in);
		return_unless(false, NULL, "reading context failed: %s\n", strerror(errno));
	}
	fclose(in);
	return result;;
}
