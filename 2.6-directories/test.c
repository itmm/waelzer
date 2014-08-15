/*>
## Unit-Tests

First we have some headers
<*/
#include "dir.h"

#include "../2.2-unit-tests/unit.h"
/*>
### Tests
#### Simple entry comparison
<*/
void t_entry_is_equal(void *context) {
	struct dirent entry = { .d_namlen = 3, .d_name = "abc" };
	assert_true(dir_entry_is(&entry, "abc"));
}
/*>
#### Simple prefix comparison
<*/
void t_entry_prefix(void *context) {
	struct dirent entry = { .d_namlen = 3, .d_name = "abc" };
	assert_true(dir_entry_has_prefix(&entry, "ab"));
}
/*>
#### Simple suffix comparison
<*/
void t_entry_suffix(void *context) {
	struct dirent entry = { .d_namlen = 3, .d_name = "abc" };
	assert_true(dir_entry_has_suffix(&entry, "bc"));
}

/*>
### Fixture
<*/
int main(int argc, char **argv) {
	run_test(t_entry_is_equal, NULL);
	run_test(t_entry_prefix, NULL);
	run_test(t_entry_suffix, NULL);
	unit_summary();
}
