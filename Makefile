COMMON_REPO := /home/centos/aws-fpga/SDAccel/examples/xilinx

#Common Includes
include $(COMMON_REPO)/utility/boards.mk
include $(COMMON_REPO)/libs/xcl2/xcl2.mk
include $(COMMON_REPO)/libs/opencl/opencl.mk


device_SRC_DIR=./interface
cpu_SRC_DIR=./cpu

SRCS=$(device_SRC_DIR)/double_buffer.cpp $(device_SRC_DIR)/device_interface.cpp $(cpu_SRC_DIR)/sha256.cpp $(cpu_SRC_DIR)/sha_preprocess.cpp main_cpu.cpp $(cpu_SRC_DIR)/verify.cpp
HDRS=$(device_SRC_DIR)/device_interface.hpp $(device_SRC_DIR)/double_buffer.hpp $(device_SRC_DIR)/defs.hpp $(cpu_SRC_DIR)/sha_preprocess.hpp $(cpu_SRC_DIR)/verify.hpp $(cpu_SRC_DIR)/sha256.hpp

# Host Application
sha256_SRCS=$(SRCS) $(oclHelper_SRCS) $(xcl2_SRCS)
sha256_HDRS=$(xcl2_HDRS) $(HDRS)
sha256_CXXFLAGS=-I$./ $(opencl_CXXFLAGS) $(xcl2_CXXFLAGS) $(oclHelper_CXXFLAGS) -std=c++0x -g
sha256_LDFLAGS=$(opencl_LDFLAGS)

EXES=sha256

# Kernel
device_kernel_SRCS=$(device_SRC_DIR)/device_kernel.cl

device_kernel_CLFLAGS= -I$(device_SRC_DIR)/ --max_memory_ports hashing_kernel

XOS=device_kernel

device_kernel_XOS=device_kernel
device_kernel_LDCLFLAGS=--xp misc:map_connect=add.kernel.hashing_kernel_1.M_AXI_GMEM0.core.OCL_REGION_0.M00_AXI --xp misc:map_connect=add.kernel.hashing_kernel_1.M_AXI_GMEM1.core.OCL_REGION_0.M01_AXI

XCLBINS=device_kernel

# check
check_EXE=sha256
check_XCLBINS=device_kernel
check_NDEVICES=$(device_kernel_NDEVICES)

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk
