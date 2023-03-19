// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XQPP_H
#define XQPP_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xqpp_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_r_BaseAddress;
} XQpp_Config;
#endif

typedef struct {
    u64 Control_r_BaseAddress;
    u32 IsReady;
} XQpp;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XQpp_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XQpp_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XQpp_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XQpp_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XQpp_Initialize(XQpp *InstancePtr, u16 DeviceId);
XQpp_Config* XQpp_LookupConfig(u16 DeviceId);
int XQpp_CfgInitialize(XQpp *InstancePtr, XQpp_Config *ConfigPtr);
#else
int XQpp_Initialize(XQpp *InstancePtr, const char* InstanceName);
int XQpp_Release(XQpp *InstancePtr);
#endif

void XQpp_Start(XQpp *InstancePtr);
u32 XQpp_IsDone(XQpp *InstancePtr);
u32 XQpp_IsIdle(XQpp *InstancePtr);
u32 XQpp_IsReady(XQpp *InstancePtr);
void XQpp_EnableAutoRestart(XQpp *InstancePtr);
void XQpp_DisableAutoRestart(XQpp *InstancePtr);

void XQpp_Set_b_in(XQpp *InstancePtr, u32 Data);
u32 XQpp_Get_b_in(XQpp *InstancePtr);
void XQpp_Set_b_out(XQpp *InstancePtr, u32 Data);
u32 XQpp_Get_b_out(XQpp *InstancePtr);
void XQpp_Set_size(XQpp *InstancePtr, u32 Data);
u32 XQpp_Get_size(XQpp *InstancePtr);
void XQpp_Set_control(XQpp *InstancePtr, u32 Data);
u32 XQpp_Get_control(XQpp *InstancePtr);

void XQpp_InterruptGlobalEnable(XQpp *InstancePtr);
void XQpp_InterruptGlobalDisable(XQpp *InstancePtr);
void XQpp_InterruptEnable(XQpp *InstancePtr, u32 Mask);
void XQpp_InterruptDisable(XQpp *InstancePtr, u32 Mask);
void XQpp_InterruptClear(XQpp *InstancePtr, u32 Mask);
u32 XQpp_InterruptGetEnabled(XQpp *InstancePtr);
u32 XQpp_InterruptGetStatus(XQpp *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
