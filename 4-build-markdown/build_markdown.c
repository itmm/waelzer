#include "../2.1-logging/log.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

char **get_list_from_content() {
	FILE *in = fopen("content.txt", "r");
	return_unless(in, NULL, "can't read content.txt");
	fclose(in);
	return NULL;
}

char **get_list_of_files() {
	return_unless(false, NULL, "not implemented");
}

int main(int argc, char **argv) {
	char **list_of_files = get_list_of_files();
	return_unless(list_of_files, EXIT_FAILURE, "can't read list of files");
	free(list_of_files);
}
