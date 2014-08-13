/*>
In the process file method we keep two bits of state:

* is the next line we read code or MarkDown?
* is the next line we write code or MarkDown?

The kind of the file sets the first flag, and we always start writing in
MarkDown mode.
<*/
static bool process_file(FILE *in, FILE *out, bool code_file) {
	bool read_in_code = code_file;
	bool write_in_code = false;
/*>
The reading looks like the reading of `context.txt`, but the lines can
be much longer. Some MarkDown tools create a line per paragraph. Maybe
we need a better method, but for a start 10k seems enough.
<*/
	static char buffer[10240]; // long lines for .md files
	while (fgets(buffer, sizeof(buffer), in)) {
/*>
If a code line starts with the special comment, we start reading
MarkDown that is inside the comment.
<*/ 
		if (code_file && strstr(buffer, "/*>") == buffer) {
			read_in_code = false;
/*>
If we are reading MarkDown in code and a line starts with the special
ending comment, we stop reading MarkDown and switch back to code.
<*/
		} else if (code_file && strstr(buffer, "<*/") == buffer) {
			read_in_code = true;
		} else {
/*>
If we have read a line of code, but are in MarkDown writing, we send a
MarkDown command to switch to code.
<*/
			if (read_in_code && !write_in_code) {
				fprintf(out, "``` c\n");
				write_in_code = true;
/*>
Likewise we switch back to MarkDown, if we are still in writing code.
<*/
			} else if (!read_in_code && write_in_code) {
				fprintf(out, "```\n");
				write_in_code = false;
			}
			fprintf(out, "%s", buffer);
		}
	}
/*>
If we ended the file in code writing mode, we need to switch back.
<*/
	if (write_in_code) {
		fprintf(out, "```\n");
	}
	return true;
}
