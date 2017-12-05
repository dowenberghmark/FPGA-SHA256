

export PYTHONHOME=/usr
export LD_LIBRARY_PATH=$XILINX_SDX/runtime/lib/x86_64/:$LD_LIBRARY_PATH
# Can target sw_emu also
export XCL_EMULATION_MODE=hw_emu
emconfigutil --platform /home/centos/aws-fpga/SDAccel/aws_platform/xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0/xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0.xpfm

# run gdb for our program - gdb --args ./sha256 test.xclbin
# source in gdb - source /opt/Xilinx/SDx/2017.1.op/scripts/appdebug.py
