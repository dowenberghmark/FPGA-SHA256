COMMON_REPO := /home/centos/aws-fpga/SDAccel/examples/xilinx

#Common Includes
include $(COMMON_REPO)/utility/boards.mk
include $(COMMON_REPO)/libs/xcl2/xcl2.mk
include $(COMMON_REPO)/libs/opencl/opencl.mk


SRC_DIR=./interface

SRCS=$(SRC_DIR)/double_buffer.cpp $(SRC_DIR)/device_interface.cpp cpu/sha256.cpp cpu/sha_preprocess.cpp main_cpu.cpp cpu/verify.cpp
HDRS=$(SRC_DIR)/device_interface.hpp $(SRC_DIR)/double_buffer.hpp $(SRC_DIR)/defs.hpp


# Host Application
sha256_SRCS=$(SRCS) $(oclHelper_SRCS) $(xcl2_SRCS)
sha256_HDRS=$(xcl2_HDRS) $(HDRS)
sha256_CXXFLAGS=-I$(SRC_DIR)/ $(opencl_CXXFLAGS) $(xcl2_CXXFLAGS) $(oclHelper_CXXFLAGS) -std=c++0x -g
sha256_LDFLAGS=$(opencl_LDFLAGS)

EXES=sha256

# Kernel
device_kernel_SRCS=$(SRC_DIR)/device_kernel.cl

device_kernel_CLFLAGS= -I$(SRC_DIR)/ --max_memory_ports hashing_kernel

XOS=device_kernel

device_kernel_XOS=device_kernel
device_kernel_LDCLFLAGS=--xp misc:map_connect=add.kernel.hashing_kernel_1.M_AXI_GMEM0.core.OCL_REGION_0.M00_AXI --xp misc:map_connect=add.kernel.hashing_kernel_1.M_AXI_GMEM1.core.OCL_REGION_0.M01_AXI --report estimate

XCLBINS=device_kernel

# check
check_EXE=sha256
check_XCLBINS=device_kernel
check_NDEVICES=$(device_kernel_NDEVICES)

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk
