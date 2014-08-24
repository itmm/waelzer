/*>
# Copy Source files

In this section, all source files from all sections are copied into a build directory. There they can be build and linked without using the relative paths in the include statements. That will ease the programming a lot.

## Structure of the Program

The program contains of the following steps:

1. Create and empty a `build` directory. 
2. Process each section directory and copy its source files to the `build` directory.

We assume a UNIX-like environment, so we can shortcut the steps of creating and emptying the `build` directory by issuing two `system` calls.
<*/
int main(int argc, char **argv) {
	return_unless(!system("mkdir -p build"), EXIT_FAILURE, "can't create dir");
	return_unless(!system("rm -Rf build/*"), EXIT_FAILURE, "can't clear build dir");
	str_lst *directories = dir_entries(".", sections_filter);
	bool result = process_directories(directories);
	str_lst_free(directories);
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
