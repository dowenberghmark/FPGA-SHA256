#!/usr/bin/env bash
flag=sw_emu

if [ $# -ne 0 ]
then
    if [ $1 == 'hw' ]
    then
	flag=hw_emu
    fi
fi

echo Usage: source with flag hw for hardware emulation, anything else uses software emulation

echo
echo Using flag $flag
echo

export LD_LIBRARY_PATH=$XILINX_SDX/runtime/lib/x86_64/:$LD_LIBRARY_PATH
export XCL_EMULATION_MODE=$flag
emconfigutil --platform /home/centos/aws-fpga/SDAccel/aws_platform/xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0/xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0.xpfm
