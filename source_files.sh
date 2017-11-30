#!/usr/bin/env bash
p=$(pwd)
cd /home/centos/aws-fpga
source hdk_setup.sh
source sdaccel_setup.sh
source /opt/Xilinx/SDx/2017.1.op/settings64.sh
cd $p

export CLOCK_A_RECIPE=0
export CLOCK_B_RECIPE=0
export CLOCK_C_RECIPE=0
export device_id=0xF001
export vendor_id=0x1D0F
export subsystem_id=0x1D51
export subsystem_vendor_id=0xFEDC
