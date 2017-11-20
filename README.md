# PineappleExpress

## Description
System that will (hopefully) accelerate SHA-256 on an FPGA. 

Amazon F1 instances will be used to test the system on real hardware. CPU and FPGA will communicate through DRAM connected to PCIe with Amazon's [EDMA driver](https://github.com/aws/aws-fpga/tree/master/sdk/linux_kernel_drivers/edma). 

A double buffer will be used to allow the CPU and FPGA to write and read buffers in parallel.

## Documentation
Code is mostly self-documenting. 

Notes from daily and weekly scrum together with other various information can be found in our [blog](https://pineappleblogg.wordpress.com/).
