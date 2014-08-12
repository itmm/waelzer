/*>
## get the list of files

We have two ways to get the files. The preferred way is to provide a `content.txt` file.
It contains a list of files. If we can't read this file, we will log an error! But to
make things easy, we will read all `*.c`, `*.h`, and `*.md` files (without `README.md`)
as a backup strategy.

The `context.txt` is clearly the preferred way, because you can provide the correct
order of the files.

The code is simple: try the `content.txt` and if it fails, load the directoy list
(which may also fail).
<*/
static str_lst *get_list_of_files() {
	str_lst *result = get_list_from_content();
	if (!result) {
		result = get_list_from_dir();
	}
	return result;
}
