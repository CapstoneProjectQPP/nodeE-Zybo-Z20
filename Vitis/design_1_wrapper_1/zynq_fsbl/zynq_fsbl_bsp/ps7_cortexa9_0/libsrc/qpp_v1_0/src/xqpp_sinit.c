// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.2 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xqpp.h"

extern XQpp_Config XQpp_ConfigTable[];

XQpp_Config *XQpp_LookupConfig(u16 DeviceId) {
	XQpp_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XQPP_NUM_INSTANCES; Index++) {
		if (XQpp_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XQpp_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XQpp_Initialize(XQpp *InstancePtr, u16 DeviceId) {
	XQpp_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XQpp_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XQpp_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

