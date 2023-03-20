/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xqpp.h"
#include "test_data.h"
#include "xil_cache.h"


#define DAT_SIZE		TEST_SIZE
#define PERMS_SIZE		PERM_LEN
#define SEED_SIZE		SEED_LEN

#define LD_PERMS		0x0F
#define LD_T_PERMS		0x81
#define LD_SEED			0xF0
#define ENCRYPT			0x3C
#define DECRYPT			0x11
#define DONE			0xCC

XQpp QPP_instance;


volatile u32 b_out[DAT_SIZE];

volatile u32 m_out[DAT_SIZE];

int main()
{
    const int STR_MAX = 100;

    init_platform();

    if(XQpp_Initialize(&QPP_instance, 0) != XST_SUCCESS) {
    	print("QPP init error\n");
    	while(1);
    }

    print("Plaintext \r\n");
    for(int i = 0; i < DAT_SIZE; i++) {
        char str[STR_MAX];
        sprintf(str, "%08x", (u32)test_data[i]);
    	xil_printf("%s", str);
    	print("\n\r");
    }

    while(!XQpp_IsReady(&QPP_instance));

    // Load Perms
    XQpp_Set_control(&QPP_instance, (u32)LD_PERMS);
    XQpp_Set_size(&QPP_instance, PERMS_SIZE);
    XQpp_Set_b_in(&QPP_instance, (u32)test_perms);
    XQpp_Start(&QPP_instance);
    while(!XQpp_IsDone(&QPP_instance));
    while(!XQpp_IsReady(&QPP_instance));



    /*
    // Load Seed
    XQpp_Set_control(&QPP_instance, (u32)LD_SEED);
    XQpp_Set_size(&QPP_instance, SEED_LEN);
    XQpp_Set_b_in(&QPP_instance, (u32)test_seed);
    XQpp_Start(&QPP_instance);
    while(!XQpp_IsDone(&QPP_instance));
    while(!XQpp_IsReady(&QPP_instance));

	*/

    // Begin Encryption
    XQpp_Set_control(&QPP_instance, (u32)ENCRYPT);
    XQpp_Set_size(&QPP_instance, DAT_SIZE);
    XQpp_Set_b_in(&QPP_instance, (u32)test_data);
    XQpp_Set_b_out(&QPP_instance, (u32)b_out);
    XQpp_Start(&QPP_instance);
    while(!XQpp_IsDone(&QPP_instance));
    while(!XQpp_IsReady(&QPP_instance));

//    for(int i = 0; i < 100000000; i++) {}
//    out = (u32*)XQpp_Get_b_out(&QPP_instance);

    Xil_DCacheInvalidateRange((INTPTR)b_out, 100 * sizeof(u32));



    // Load Transpose Perms
    XQpp_Set_control(&QPP_instance, (u32)LD_T_PERMS);
    XQpp_Set_size(&QPP_instance, PERMS_SIZE);
    XQpp_Set_b_in(&QPP_instance, (u32)test_t_perms);
    XQpp_Start(&QPP_instance);
    while(!XQpp_IsDone(&QPP_instance));
    while(!XQpp_IsReady(&QPP_instance));

    // Begin Decryption
    XQpp_Set_control(&QPP_instance, (u32)DECRYPT);
    XQpp_Set_size(&QPP_instance, DAT_SIZE);
    XQpp_Set_b_in(&QPP_instance, (u32)b_out);
    XQpp_Set_b_out(&QPP_instance, (u32)m_out);
    XQpp_Start(&QPP_instance);
    while(!XQpp_IsDone(&QPP_instance));
    while(!XQpp_IsReady(&QPP_instance));


    print("Encryption complete, printing results...\n\r");

    for(int i = 0; i < DAT_SIZE; i++) {
        char str[STR_MAX];
        sprintf(str, "%08x", (u32)b_out[i]);
    	xil_printf("%s", str);
    	print("\n\r");
    }

    print("Decryption complete, printing results...\n\r");

    for(int i = 0; i < DAT_SIZE; i++) {
        char str[STR_MAX];
        sprintf(str, "%08x", (u32)m_out[i]);
    	xil_printf("%s", str);
    	print("\n\r");
    }

    cleanup_platform();
    return 0;
}
