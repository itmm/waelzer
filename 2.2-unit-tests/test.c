#include "unit.h"

void t_trivial(void *context) {}

int main(int argc, char **argv) {
	run_test(t_trivial, NULL);
	unit_summary();
}
