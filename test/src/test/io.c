
#include "io.h"

void io_setup() {
	default_setup();
	input_setup();
	output_setup();
}

void io_teardown() {
	default_teardown();
	input_teardown();
	output_teardown();
}