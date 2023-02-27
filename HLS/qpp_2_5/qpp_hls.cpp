#include "qpp_hls.h"

/*
*
* https://github.com/Xilinx/Vitis-HLS-Introductory-Examples/blob/master/Task_level_Parallelism/Control_driven/Channels/using_fifos/diamond.cpp
*
*/

void encrypt_word(hls::stream<word_t>& in, hls::stream<word_t>& out) {

	word_t wrd = in.read();
//	word_t x = (ap_uint<n>) seed[i*n];
	word_t x = 0b10;
	word_t m = wrd ^ x;
//	uint8_t encoding = (uint8_t)m;
//	word_t c = perms[x % M][encoding];

	out.write(m);
}

void encrypt_block(hls::stream<block_t>& in, hls::stream<block_t>& out, int size) {

	block_t output_block;

	hls::stream<word_t> m;
	hls::stream<word_t> x;
	hls::stream<word_t> c;

#pragma HLS PIPELINE II=10
	block_t text_block = in.read();
	for(int j = 0; j < size; j++) {
		for(int i = 0; i < BLOCK_SIZE/n; i++) {
			m.write(text_block[i*n]);
//			x.write((ap_uint<n>) seed[i*n]);
//			encrypt_word(&m, &x, &c);
			encrypt_word(m, c);

			output_block[i*n] = c.read();
		}
		out.write(output_block);
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


void load(block_t *b_in, hls::stream<block_t >& out, int size) {
	for(int i = 0; i < size; i++) {

		out.write(b_in[i]);
	}
}

void store(hls::stream<block_t >& in, block_t *b_out, int size) {
	for(int i = 0; i < size; i++) {

		b_out[i] = in.read();
	}
}

void encrypt(block_t *b_in, block_t *b_out, int size) {
	hls::stream<block_t> c0, c1;

#pragma HLS DATAFLOW
	load(b_in, c0, size);
	encrypt_block(c0, c1, size);
	store(c1, b_out, size);
}


extern "C" {

void qpp(block_t* b_in, block_t *b_out, int *control, int size) {

#pragma HLS INTERFACE mode=m_axi bundle=BUS_A port=b_out depth=32
#pragma HLS INTERFACE mode=m_axi bundle=BUS_A port=b_in depth=32
#pragma HLS INTERFACE mode=s_axilite port=control bundle=ctrl
#pragma HLS INTERFACE mode=s_axilite port=size bundle=ctrl
#pragma HLS INTERFACE mode=s_axilite port=return bundle=ctrl

	ap_uint<dim> perms[5][dim];
	ap_uint<SEED_LEN> seed;


	if(*control == 0x0F) {
//		load_perms(b_in, perms);
	} else if (*control == 0xF0) {
//		load_seed(b_in, seed);
	} else if (*control == 0x3C) {

		encrypt(b_in, b_out, size);

	}
}

}

