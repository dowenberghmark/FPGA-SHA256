COMMON_REPO := ../../..

#Common Includes
include $(COMMON_REPO)/utility/boards.mk
include $(COMMON_REPO)/libs/xcl2/xcl2.mk
include $(COMMON_REPO)/libs/opencl/opencl.mk

SRC_DIR=./src
SRCS=$(SRC_DIR)/cpu_dummy.cpp $(SRC_DIR)/device_interface.cpp $(SRC_DIR)/double_buffer.cpp
SRCS=$(SRC_DIR)/device_interface.hpp $(SRC_DIR)/double_buffer.hpp

# Host Application
sha256_SRCS=./src/host.cpp $(oclHelper_SRCS) $(xcl2_SRCS)
sha256_HDRS=$(xcl2_HDRS)
sha256_CXXFLAGS=-I./src/ $(opencl_CXXFLAGS) $(xcl2_CXXFLAGS) $(oclHelper_CXXFLAGS) -std=c++0x
sha256_LDFLAGS=$(opencl_LDFLAGS)

EXES=sha256

# Kernel
device_interface_SRCS=./src/device_interface.cl

XOS=device_interface

device_interface_XOS=device_interface

XCLBINS=device_interface

# check
check_EXE=sha256
check_XCLBINS=device_interface

CHECKS=check

include $(COMMON_REPO)/utility/rules.mk
