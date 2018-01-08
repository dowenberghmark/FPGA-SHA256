COMMON_REPO := /home/centos/aws-fpga/SDAccel/examples/xilinx

#Common Includes
include $(COMMON_REPO)/utility/boards.mk
include $(COMMON_REPO)/libs/xcl2/xcl2.mk
include $(COMMON_REPO)/libs/opencl/opencl.mk

device_SRC_DIR=./device
host_SRC_DIR=./host

SRCS=$(device_SRC_DIR)/double_buffer.cpp $(device_SRC_DIR)/device_interface.cpp $(host_SRC_DIR)/host_sha256.cpp $(host_SRC_DIR)/sha_preprocess.cpp main_cpu.cpp $(host_SRC_DIR)/verify.cpp
HDRS=$(device_SRC_DIR)/device_interface.hpp $(device_SRC_DIR)/double_buffer.hpp $(device_SRC_DIR)/defs.hpp $(host_SRC_DIR)/sha_preprocess.hpp $(host_SRC_DIR)/verify.hpp $(host_SRC_DIR)/host_sha256.hpp

# Host Application
sha256_SRCS=$(SRCS) $(oclHelper_SRCS) $(xcl2_SRCS)
sha256_HDRS=$(xcl2_HDRS) $(HDRS)
sha256_CXXFLAGS=-I$./ $(opencl_CXXFLAGS) $(xcl2_CXXFLAGS) $(oclHelper_CXXFLAGS) -std=c++0x -g
sha256_LDFLAGS=$(opencl_LDFLAGS) -lcrypto

EXES=sha256

# Kernel
device_kernel_SRCS=$(device_SRC_DIR)/device_kernel.cl

device_kernel_CLFLAGS= -I$(device_SRC_DIR)/ --max_memory_ports hashing_kernel

XOS=device_kernel

device_kernel_XOS=device_kernel
device_kernel_LDCLFLAGS=--xp misc:map_connect=add.kernel.hashing_kernel_1.M_AXI_GMEM0.core.OCL_REGION_0.M00_AXI

XCLBINS=device_kernel

# check
check_EXE=sha256
check_XCLBINS=device_kernel
check_NDEVICES=$(device_kernel_NDEVICES)

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk
