/*>
## Processing the input files

So now we can start to process the files. Let's jump to the `main` function and see the good parts. First we get files to process and open the output file.

If there are no input files, we terminate and wont overwrite an existing `README.md` file (just be save).
<*/
int main(int argc, char **argv) {
	str_lst *files = get_list_of_files();
	return_unless(files, EXIT_FAILURE, "can't read list of files");
	return_unless(str_lst_count(files), EXIT_SUCCESS, "nothing to process");

	FILE *out = fopen("README.md", "w");
	if (!out) {
		str_lst_free(files);
		return_unless(false, EXIT_FAILURE, "can't open README.md");
	}
/*>
Now we open each file and call the process function. The function needs to know, if the file contains MarkDown directly, or is a source code file, that only contains MarkDown in special comment blocks.

We do the suffix magic to check, if it ends in `.md`. Otherwise we will assume it a source code file.
<*/
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
