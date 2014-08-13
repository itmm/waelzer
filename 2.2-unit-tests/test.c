/*>
# Unit-Tests

I love test-driven development, but it is hard to achieve. The most important part, 
is the testing framework. Here we use a very minimal framework, just for this project.

Each test is a function, that can get a context passed along. There are setup and 
tear down functions that can be invoked before and after any test.

If the test fails, we jump out of the test and continue with the next test.

The following code shows all, that should be needed to run a test:
<*/
#include "unit.h"

void t_trivial(void *context) {}

int main(int argc, char **argv) {
	run_test(t_trivial, NULL);
	unit_summary();
}
