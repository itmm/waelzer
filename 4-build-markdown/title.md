# Generating the MarkDown file

In this section we will generate the MarkDown file `README.md`. The name is choosen, so it will be displayed directly in GitHub.

Basically we do three interesting things:

* get the list of files to process from the `context.txt` file, if it exists,
* get the list of files from the directory, if the `context.txt` file doesn't exist, and
* parse all the files to produce one big `README.md` file.

Before processing the markdown, let's start with the most easy stuff: building a `str_lst` of all the files that should be processed.

