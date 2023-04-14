#include "qpp_hls.h"

#define TEST_SIZE 3


block_t test_seed[1] = { 0xF0F0F0F0 };

/*
 * Hello World
 * H = 0x43, e = 0x65, l = 0x6c, l = 0x6c, o = 0x6f, NULL = 0x00,
 * space = 0x20, W = 0x57, o = 0x6f, r = 0x72, l = 0x6c, d = 0x64
 * In 32 bit blocks : 0x43656c6c, 0x6f002057, 0x6f726c64
 */
block_t test_data[TEST_SIZE] = {
		0x48656c6c, 0x6f002057, 0x6f726c64
};


/*
* perm 0 : 01, 10, 00, 11 = 0x63
* perm 1 : 10, 01, 00, 11 = 0x93
* perm 2 : 01, 11, 10, 00 = 0x78
* perm 3 : 00, 11, 10, 01 = 0x39
* perm 4 : 00, 11, 01, 10 = 0x36
* In 32 bit blocks : 0x63937839 , 0x36000000 <- padded with zeros
*/
block_t test_perms[2] = {
		0x63937839, 0x36000000
};


/*
* perm 0 : 10, 00, 01, 11 = 0x87
* perm 1 : 10, 01, 00, 11 = 0x93
* perm 2 : 11, 00, 10, 01 = 0xC9
* perm 3 : 00, 11, 10, 01 = 0x39
* perm 4 : 00, 10, 11, 01 = 0x2D
* In 32 bit blocks : 0x8793C939 , 0x2D000000 <- padded with zeros
*/
block_t test_t_perms[2] = {
		0x8793C939 , 0x2D000000
};


block_t test_result[TEST_SIZE] = {
		0x95b2bdbd, 0x335575ab, 0xb385b2ba
};

block_t test_result_old[TEST_SIZE] = {
		0xA8C7DEE9, 0x666409FB, 0xEB4180C5
};


