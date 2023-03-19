#include "test_data.h"
#include "qpp_hls.h"
#include <iostream>
#include <stdio.h>

bool check_result(block_t res[TEST_SIZE], block_t com[TEST_SIZE]) {
	bool result = true;
    for(int i = 0; i < TEST_SIZE; i++) {
		printf("%08x\n", (uint32_t)res[i]);
		if(res[i] != com[i]) {
			result = false;
		}
    }
	return result;
}

int main() {

	block_t test_out[TEST_SIZE];
	block_t helloworld[TEST_SIZE];
	printf("Plain text : \n");
	for(int i = 0; i < TEST_SIZE; i++) {
		printf("%08x\n", (uint32_t)test_data[i]);

	}
	int ctrl;

	ctrl = 0x0F;

	qpp(test_perms, test_out, 2, &ctrl);

//	ctrl = 0xF0;
//
//	qpp(&test_seed, test_out, 1, &ctrl);

	ctrl = 0x81;

	qpp(test_t_perms, test_out, 2, &ctrl);


	ctrl = 0x3C;

	qpp(test_data, test_out, TEST_SIZE, &ctrl);

	printf("Cypher text : \n");
	bool result1 = check_result(test_out, test_result);

	ctrl = 0x11;

	qpp(test_out, helloworld, TEST_SIZE, &ctrl);

	printf("Plain text : \n");
	bool result2 = check_result(helloworld, test_data);


	if(result1 == true) {
		printf("encrypt passed\n");
	} else {
		printf("encrypt failed\n");
	}

	if(result2 == true) {
		printf("decrypt passed\n");
	} else {
		printf("decrypt failed\n");
	}

	return 0;
}
