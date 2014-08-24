/*>
## Unit-Tests

We need some headers.
<*/
#include "str.h"

#include "../2.2-unit-tests/unit.h"
/*>
A special tear down function can be used to assure, that the memory of allocated strings is freed, even when the test failed.
<*/
static char *str = NULL;

static void teardown(void *context) {
	str = str_free(str);
}
/*>
### Concatenation tests
#### Can we concatenate a couple of strings?
<*/
static void t_simple(void *context) {
	str = str_cons(3, "a", "b", "c");
	assert_str(str, "abc");
}
/*>
#### Even if some entries are `NULL`?
<*/
static void t_null_entries(void *context) {
	str = str_cons(5, "a", NULL, "b", NULL, "c");
	assert_str(str, "abc");
}
/*>
#### How about zero entries?
<*/
static void t_empty(void *context) {
	str = str_cons(0);
	assert_str(str, "");
}
/*>
#### Or only one `NULL` entry?
<*/
static void t_only_NULL(void *context) {
	str = str_cons(1, NULL);
	assert_str(str, "");
}
/*>
### Freeing tests
#### Can we free `NULL`?
<*/
void t_free_NULL(void *context) {
	str_free(NULL);
}
/*>
#### Or the empty string?
<*/
void t_free_empty(void *context) {
	str_free(str_empty());
}
/*>
### Setup
<*/
int main(int argc, char **argv) {
	run_test_ex(t_simple, NULL, NULL, teardown);
	run_test_ex(t_null_entries, NULL, NULL, teardown);
	run_test_ex(t_empty, NULL, NULL, teardown);
	run_test_ex(t_only_NULL, NULL, NULL, teardown);
	run_test(t_free_NULL, NULL);
	run_test(t_free_empty, NULL);
	unit_summary();
}
