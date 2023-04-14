//===========================================================================
// qpp_hls.h
//===========================================================================
// @brief: This header file defines the interface for the core functions.

#ifndef QPPHLS_H
#define QPPHLS_H

#include "ap_int.h"
#include "stdint.h"
#include <hls_stream.h>

#define n 2
#define dim 1 << n
#define M 5
#define N n * M * dim
#define BUS_WIDTH 32


typedef ap_uint<1> bit_t;
typedef ap_uint<n> word_t;

typedef ap_uint<BUS_WIDTH> block_t;


extern "C" {
// top function
	void qpp(block_t* b_in, block_t *b_out, const int size, int *control);
}


void encrypt_word(hls::stream<word_t>& in, hls::stream<word_t>& out);

void encrypt_block(hls::stream<block_t>& in, hls::stream<block_t>& out, int size);

void load(hls::stream<block_t >& in, block_t *b_out, int size);

void store(block_t *b_in, hls::stream<block_t >& out, int size);

void load_perms(block_t* b_in, int size, ap_uint<n> perms[5][dim]);

block_t prng();

#endif
