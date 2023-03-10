#include "qpp_hls.h"
#include "hls_print.h"


ap_uint<n> perms[5][dim];
ap_uint<32> seed;



void encrypt_word(word_t &word, word_t &x, word_t &c) {
	word_t m = word ^ x;
	c = perms[x % M][(int)m];
}


void encrypt_block(block_t* b_in, block_t *b_out, const int block_size) {
	for(int i = 0; i < block_size; i++) {
#pragma HLS PIPELINE II=1
		block_t block = b_in[i];
		block_t out_block = 0x00000000;
		block_t rn = prng();
		for(int j = 0; j < BUS_WIDTH/n; j++) {
			word_t m = (block >> (j*2)) & 0x03;
			word_t c;
			word_t x = (rn >> (j*2)) & 0x03;
			encrypt_word(m, x, c);
			out_block |= (uint32_t)c << (j*2);
		}

		b_out[i] = out_block;

	}
}


void load_perms(block_t* b_in) {
	// each permutation matrix has 2^n elements of size n bits. Since n = 2, and (n * 2^n = 8), there
	// are 8 bits per permuation matrix. There are 32 bits in a block (BUS_WIDTH), so one block will
	// contain the elements for multiple permutation matrices.
	int block_num = 0;
	int block_pos = 0;
	perms_loop1: for(int i = 0; i < M; i++) {
		perms_loop2: for(int j = 0; j < dim; j++) {
			// lead each element of the matrix from input block
			perms[i][j] |= (ap_uint<n>)((b_in[block_num] >> (block_pos*2)) & 0x3);
			block_pos++;
			// move block number and reset position once all bits from current block have been read
			if(block_pos == BUS_WIDTH/2) {
				block_num++;
				block_pos = 0;
			}
		}
	}
}


/*
* https://highlevel-synthesis.com/2017/02/10/lfsr-in-hls/
* x^32 + x^22 + x^2 + x + 1
*/

block_t prng() {

  bool b_32 = seed.get_bit(32-32);
  bool b_22 = seed.get_bit(32-22);
  bool b_2 = seed.get_bit(32-2);
  bool b_1 = seed.get_bit(32-1);
  bool new_bit = b_32 ^ b_22 ^ b_2 ^ b_1;
  seed = seed >> 1;
  seed.set_bit(31, new_bit);

  return seed;

}


extern "C" {

void qpp(block_t* b_in, block_t *b_out, const int size, int *control) {



    #pragma HLS INTERFACE m_axi port=b_in offset=slave max_read_burst_length=16
    #pragma HLS INTERFACE m_axi port=b_out offset=off max_write_burst_length=16
    #pragma HLS INTERFACE s_axilite port=size bundle=CRTL_BUS
    #pragma HLS INTERFACE s_axilite port=control bundle=CRTL_BUS
    #pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS


	if(*control == 0x0F) {
		load_perms(b_in);
		*control = 0x0B;
	} else if (*control == 0xF0) {
		seed = b_in[0];
		*control = 0x0A;
	} else if (*control == 0x3C) {
		encrypt_block(b_in, b_out, size);
		*control = 0xFC;
	}
}

}

