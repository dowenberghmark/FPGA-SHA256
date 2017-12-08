#!/usr/bin/env bash
hw_flag=$1
if [ $# -eq 0 ]
then
    hw_flag='sw'
fi

echo Usage: add flag hw for hardware emulation, anything else uses software emulation
export LD_LIBRARY_PATH=$XILINX_SDX/runtime/lib/x86_64/:$LD_LIBRARY_PATH # Can target sw_emu also

if [ $hw_flag == 'hw' ]
then
    echo Running with hardware emulation
    export XCL_EMULATION_MODE=hw_emu
else
    echo Running with software emulation
    export XCL_EMULATION_MODE=sw_emu
fi

emconfigutil --platform /home/centos/aws-fpga/SDAccel/aws_platform/xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0/xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0.xpfm

