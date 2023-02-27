#include "qpp_hls.h"

/*
*
* https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/master/Task_level_Parallelism/Control_driven/Channels/using_fifos/diamond.cpp
*
*/

void encrypt_word(word_t &word, word_t &c) {
	word_t m = word ^ 0b10;
//	uint8_t encoding = (uint8_t)m;
//	word_t c = perms[x % M][encoding];
	c = m;
}

void encrypt_block(block_t* b_in, block_t *b_out, int block_size) {

	block_t output_block;

	hls::stream<word_t> m;
	hls::stream<word_t> x;
	hls::stream<word_t> c;

#pragma HLS PIPELINE II=1
	for(int i = 0; i < block_size/n; i++) {
		block_t block = b_in[i];
		block_t out_block;
		for(int j = 0; j < BUS_WIDTH/n; i++) {
			word_t m = (block >> (j+1*2)) & 0x03;
			word_t c;
			encrypt_word(m, c);
			out_block |= c >> (j+1*2);
		}
		b_out[i] = out_block;
	}
}


//void load_perms(block_t* b_in, ap_uint<dim> &perms[5][dim]) {
//	for(int j = 0; j < size; j++) {
//
//		out.write(output_block);
//	}
//}
//
//void load_seed(block_t* b_in, ap_uint<dim> &perms[5][dim]) {
//	for(int j = 0; j < size; j++) {
//
//		out.write(output_block);
//	}
//}

//void prng(ap_uint<SEED_LEN> &seed) {
//
//}




extern "C" {

void qpp(block_t* b_in, block_t *b_out, int *control, int size) {


    #pragma HLS INTERFACE m_axi port=b_in max_read_burst_length=16
    #pragma HLS INTERFACE m_axi port=b_out max_write_burst_length=16
    #pragma HLS INTERFACE s_axilite port=size bundle=CRTL_BUS
    #pragma HLS INTERFACE s_axilite port=control bundle=CRTL_BUS
    #pragma HLS INTERFACE s_axilite port=return bundle=CRTL_BUS

	ap_uint<dim> perms[5][dim];
	ap_uint<SEED_LEN> seed;


	if(*control == 0x0F) {
//		load_perms(b_in, perms);
	} else if (*control == 0xF0) {
//		load_seed(b_in, seed);
	} else if (*control == 0x3C) {

		encrypt_block(b_in, b_out, size);

	}
}

}

