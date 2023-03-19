# nodeE-Zybo-Z20
Encryption node project on Zybo Z20 board

This repository contains source files for three types of projects: Vitis HLS, Vivado, and Vitis IDE all version 2021.2. A README located in each subdirectory contains instructions for building the respective project.

# HLS

Vitis HLS is used for generating the RTL for the encrypt/decrypt kernels using a high-level C language description. A given kernel contains a source file with a top-level function, a header file containing type defintions and defines, a testbench source file for simulating the kernel functionality, and a test data header file to supply data to the testbench. After simulating the kernel, RTL can be generated by a synthesis command, and the RTL can be exported for use in a Vivado or Vitis design flow. This repository uses the Vivado design flow.

# Vivado
