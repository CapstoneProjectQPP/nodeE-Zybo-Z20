#include "test_data.h"
#include "qpp_hls.h"
#include <iostream>
#include <stdio.h>

bool check_result(block_t res[TEST_SIZE]) {
	for(int i = 0; i <= TEST_SIZE; i++) {
		printf("%d\n", res[i]);
		if(res[i] != test_result) {
			return false;
		}
	}
	return true;
}

int main() {

	block_t test_out[TEST_SIZE];
	for(int i = 0; i < TEST_SIZE; i++) {
		printf("%d\n", (int)test_data[i]);

	}
	int ctrl;

	ctrl = 0x0F;

	qpp(test_perms, test_out, 2, &ctrl);

	while(ctrl != 0x0B);

	ctrl = 0xF0;

	qpp(&test_seed, test_out, 1, &ctrl);

	while(ctrl != 0x0A);



	ctrl = 0x3C;

	qpp(test_data, test_out, TEST_SIZE, &ctrl);

//	bool result = check_result(test_out);

	printf("%d\n", (int)ctrl);
	bool result = true;
	for(int i = 0; i < TEST_SIZE; i++) {
		printf("%d\n", (int)test_out[i]);
		if((int)test_out[i] != (int)test_result[i]) {
			result = false;
		}
	}
	if(result == true) {
		printf("test passed\n");
	} else {
		printf("test failed\n");
	}

	return 0;
}
