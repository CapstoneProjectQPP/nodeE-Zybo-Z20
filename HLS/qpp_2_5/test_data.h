#include "qpp_hls.h"

#define TEST_SIZE 3

/*
 * Hello World
 * H = 0x43, e = 0x65, l = 0x6c, l = 0x6c, o = 0x6f, NULL = 0x00,
 * space = 0x20, W = 0x57, o = 0x6f, r = 0x72, l = 0x6c, d = 0x64
 * In 32 bit blocks : 0x43656c6c, 0x6f002057, 0x6f726c64
 */
block_t test_data[TEST_SIZE] = {
		0x43656c6c, 0x6f002057, 0x6f726c64
};


///*
//* perm 0 : 01, 10, 00, 11 = 0x63
//* perm 1 : 10, 01, 00, 11 = 0x93
//* perm 2 : 01, 11, 10, 00 = 0x78
//* perm 3 : 00, 11, 10, 01 = 0x39
//* perm 4 : 00, 11, 01, 10 = 0x36
//* In 32 bit blocks : 0x63937939 , 0x36000000 <- padded with zeros
//*/
//block_t test_perms[2] = {
//		0x63937839, 0x36000000
//};
//
//
///*
//* perm 0 : 10, 00, 01, 11 = 0x87
//* perm 1 : 10, 01, 00, 11 = 0x93
//* perm 2 : 11, 00, 10, 01 = 0xC9
//* perm 3 : 00, 11, 10, 01 = 0x39
//* perm 4 : 00, 10, 11, 01 = 0x2D
//* In 32 bit blocks : 0x8793C939 , 0x2D000000 <- padded with zeros
//*/
//block_t test_t_perms[2] = {
//		0x8793C939 , 0x2D000000
//};


/*
* perm 0 : 01, 10, 00, 11 = 0x63
* perm 1 : 10, 01, 00, 11 = 0x93
* perm 2 : 01, 11, 10, 00 = 0x78
* perm 3 : 00, 11, 10, 01 = 0x39
* perm 4 : 00, 11, 01, 10 = 0x36
*
* perm 0 : 10, 00, 01, 11 = 0x87
* perm 1 : 10, 01, 00, 11 = 0x93
* perm 2 : 11, 00, 10, 01 = 0xc9
* perm 3 : 00, 11, 10, 01 = 0x39
* perm 4 : 00, 10, 11, 01 = 0x2d
* In 32 bit blocks : 0x8793c939 , 0xd000000 <- padded with zeros
*/
block_t test_perms[2] = {
		0x8793c939, 0x2d000000
};


/*
* perm 0 : 10, 00, 01, 11 = 0x87
* perm 1 : 10, 01, 00, 11 = 0x93
* perm 2 : 11, 00, 10, 01 = 0xC9
* perm 3 : 00, 11, 10, 01 = 0x39
* perm 4 : 00, 10, 11, 01 = 0x2D
*
* perm 0 : 01, 10, 00, 11 = 0x63
* perm 1 : 10, 01, 00, 11 = 0x93
* perm 2 : 01, 11, 10, 00 = 0x78
* perm 3 : 00, 11, 10, 01 = 0x39
* perm 4 : 00, 11, 01, 10 = 0x36
* In 32 bit blocks : 0x63937839 , 0x36000000 <- padded with zeros
*/
block_t test_t_perms[2] = {
		0x63937839 , 0x36000000
};


/*
 * 0xA8, 0xC7, 0xDE, 0x96, 0x66, 0x64, 0x09, 0xFB, 0xEB, 0x41, 0x80, 0xC5
 * In 32 bit blocks : 0xA8C7DE96, 0x666409FB, 0xEB4180C5
 */
block_t test_result[TEST_SIZE] = {
		0xA8C7DE96, 0x666409FB, 0xEB4180C5
};



