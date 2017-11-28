COMMON_REPO := /home/centos/aws-fpga/SDAccel/examples/xilinx

#Common Includes
include $(COMMON_REPO)/utility/boards.mk
include $(COMMON_REPO)/libs/xcl2/xcl2.mk
include $(COMMON_REPO)/libs/opencl/opencl.mk

SRC_DIR=./interface
SRCS=$(SRC_DIR)/cpu_dummy.cpp $(SRC_DIR)/device_interface.cpp $(SRC_DIR)/double_buffer.cpp
HDRS=$(SRC_DIR)/device_interface.hpp $(SRC_DIR)/double_buffer.hpp

# Host Application
sha256_SRCS=$(SRCS) $(oclHelper_SRCS) $(xcl2_SRCS)
sha256_HDRS=$(xcl2_HDRS) $(HDRS)
sha256_CXXFLAGS=-I./src/ $(opencl_CXXFLAGS) $(xcl2_CXXFLAGS) $(oclHelper_CXXFLAGS) -std=c++0x
sha256_LDFLAGS=$(opencl_LDFLAGS)

EXES=sha256

# Kernel
device_kernel_SRCS=$(SRC_DIR)/device_kernel.cl

XOS=device_kernel

device_kernel_XOS=device_kernel

XCLBINS=device_kernel

# check
check_EXE=sha256
check_XCLBINS=device_kernel

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk
