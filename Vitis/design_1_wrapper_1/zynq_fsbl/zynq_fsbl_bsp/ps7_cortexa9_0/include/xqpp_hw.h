// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control_r
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        bit 5 - enable ap_local_deadlock interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (COR/TOW)
//        bit 1 - ap_ready (COR/TOW)
//        bit 5 - ap_local_deadlock (COR/TOW)
//        others - reserved
// 0x10 : Data signal of b_in
//        bit 31~0 - b_in[31:0] (Read/Write)
// 0x14 : reserved
// 0x18 : Data signal of b_out
//        bit 31~0 - b_out[31:0] (Read/Write)
// 0x1c : reserved
// 0x20 : Data signal of size
//        bit 31~0 - size[31:0] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of control
//        bit 31~0 - control[31:0] (Read/Write)
// 0x2c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XQPP_CONTROL_R_ADDR_AP_CTRL      0x00
#define XQPP_CONTROL_R_ADDR_GIE          0x04
#define XQPP_CONTROL_R_ADDR_IER          0x08
#define XQPP_CONTROL_R_ADDR_ISR          0x0c
#define XQPP_CONTROL_R_ADDR_B_IN_DATA    0x10
#define XQPP_CONTROL_R_BITS_B_IN_DATA    32
#define XQPP_CONTROL_R_ADDR_B_OUT_DATA   0x18
#define XQPP_CONTROL_R_BITS_B_OUT_DATA   32
#define XQPP_CONTROL_R_ADDR_SIZE_DATA    0x20
#define XQPP_CONTROL_R_BITS_SIZE_DATA    32
#define XQPP_CONTROL_R_ADDR_CONTROL_DATA 0x28
#define XQPP_CONTROL_R_BITS_CONTROL_DATA 32

