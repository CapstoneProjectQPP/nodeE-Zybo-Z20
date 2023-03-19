// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xqpp.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XQpp_CfgInitialize(XQpp *InstancePtr, XQpp_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_r_BaseAddress = ConfigPtr->Control_r_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XQpp_Start(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_AP_CTRL) & 0x80;
    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_AP_CTRL, Data | 0x01);
}

u32 XQpp_IsDone(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XQpp_IsIdle(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XQpp_IsReady(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XQpp_EnableAutoRestart(XQpp *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_AP_CTRL, 0x80);
}

void XQpp_DisableAutoRestart(XQpp *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_AP_CTRL, 0);
}

void XQpp_Set_b_in(XQpp *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_B_IN_DATA, Data);
}

u32 XQpp_Get_b_in(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_B_IN_DATA);
    return Data;
}

void XQpp_Set_b_out(XQpp *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_B_OUT_DATA, Data);
}

u32 XQpp_Get_b_out(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_B_OUT_DATA);
    return Data;
}

void XQpp_Set_size(XQpp *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_SIZE_DATA, Data);
}

u32 XQpp_Get_size(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_SIZE_DATA);
    return Data;
}

void XQpp_Set_control(XQpp *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_CONTROL_DATA, Data);
}

u32 XQpp_Get_control(XQpp *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_CONTROL_DATA);
    return Data;
}

void XQpp_InterruptGlobalEnable(XQpp *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_GIE, 1);
}

void XQpp_InterruptGlobalDisable(XQpp *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_GIE, 0);
}

void XQpp_InterruptEnable(XQpp *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_IER);
    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_IER, Register | Mask);
}

void XQpp_InterruptDisable(XQpp *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_IER);
    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_IER, Register & (~Mask));
}

void XQpp_InterruptClear(XQpp *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XQpp_WriteReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_ISR, Mask);
}

u32 XQpp_InterruptGetEnabled(XQpp *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_IER);
}

u32 XQpp_InterruptGetStatus(XQpp *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XQpp_ReadReg(InstancePtr->Control_r_BaseAddress, XQPP_CONTROL_R_ADDR_ISR);
}

