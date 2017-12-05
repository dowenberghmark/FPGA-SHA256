# Amazon FPGA Hardware Development Kit
#
# Copyright 2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#
# Licensed under the Amazon Software License (the "License"). You may not use
# this file except in compliance with the License. A copy of the License is
# located at
#
#    http://aws.amazon.com/asl/
#
# or in the "license" file accompanying this file. This file is distributed on
# an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, express or
# implied. See the License for the specific language governing permissions and
# limitations under the License.

# Extended by student group PineappleExpress to allow for different compilation modes.
# Run either 'make mode=local' or 'make mode=aws' for the different use cases.


CC = g++
CFLAGS = -DCONFIG_LOGLEVEL=4 -std=c++11 -g -Wall #-Werror

LDLIBS = -lpthread

SRC = interface/double_buffer.cpp interface/utils.cpp cpu/sha256.cpp cpu/sha_preprocess.cpp  main_cpu.cpp cpu/verify.cpp shatest/sha256.c

#Compiler flags
#if mode variable is empty, setting local build mode
ifeq ($(mode),aws)
	INCLUDES = -I$(SDK_DIR)/userspace/include
	CFLAGS += $(INCLUDES) -DMODE=1
	LDLIBS += -lfpga_mgmt -lrt
	BIN = aws_interface_main
else
	CFLAGS += -DMODE=0
	SRC += interface/fpga.cpp
	BIN = main
endif

OBJ = $(SRC:.c=.o)

all: check_env $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o $(BIN)

check_env:
ifeq ($(mode),aws)
        ifndef SDK_DIR
	    $(error SDK_DIR is undefined. Try "source sdk_setup.sh" to set the software environment)
        endif
endif
