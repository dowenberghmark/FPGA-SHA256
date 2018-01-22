[![Build Status](https://travis-ci.org/dowenberghmark/FPGA-SHA256.svg?branch=master)](https://travis-ci.org/dowenberghmark/FPGA-SHA256)

# PineappleExpress

## Description
System that will (hopefully) accelerate SHA-256 on an FPGA.

Amazon F1 instances will be used to test the system on real hardware. CPU and FPGA will communicate through DRAM connected to PCIe with ~~Amazon's EDMA driver~~ an XDMA driver through OpenCL. Both host code and FPGA kernel will be written in OpenCL C/C++ and compiled using Xilinx's SDAccel tool.

A double buffer will be used to allow the CPU and FPGA to write and read buffers in parallel.

## Documentation
Code is mostly self-documenting.

Notes from daily and weekly scrum together with other various information can be found in our [blog](https://pineappleblogg.wordpress.com/).

## Building PineappleExpress
From an FPGA Developer AMI, clone the repository, cd into it and run:

```
# setup environment for building
source source_files.sh
# make and run program
./run_make
# set up environment for stand alone program
source enable_manual_run.sh
# run stand alone program
./sha256
```

Default target is software emulation. For hardware emulation simply add a `hw` flag to `./run_make` and `source enable_manual_run.sh`.

## Run PineappleExpress
Use ./sha256 -h for more help on how to run the program.

```
Example:
./sha256 -d -f foo.txt -s 100
```
```
Useful options:
-b  Activates benchmark mode. Will append run time info to results/output.csv if -o flag is not used.
-o  Specifies output file for benchmarking. The output file is set to results/output.csv as default.
-v  Activates verification mode. Verifies results to a third-party program.
-f  Specify which file to read. The program will read password.txt if the flag is not specified.
-s  Specify how many Megabytes to read. The whole file will be read if the flag is not specified.
-d  Activates debug mode.
-B  Defines buffer size in MB. Default is four chunks per buffer.
-h  Help page.
```

## Run Benchmarks
Benchmarks can be generated for both device and host.

Generate enough passwords (100 GB required for benchmarks script) with:

`python host/generate_random_passwords.py 100000`

Compile the wanted target. For host:

`cd host && make`

For device:
```
source source_files.sh
make clean
make TARGETS=hw DEVICES=$AWS_PLATFORM all
```
Wait ~8 hours for it to finish, then follow these [instructions](https://github.com/aws/aws-fpga/blob/master/SDAccel/README.md#createafi) to upload the compiled kernel to AWS and then these [instructions](https://github.com/aws/aws-fpga/tree/master/hdk/cl/examples#step-by-step-guide-how-to-load-and-test-a-registered-afi-from-within-an-f1-instance) to load the FPGA on the F1 instance.

Run the benchmarks with:

`./benchmark.sh myoutput.csv random_passwords.txt <host/hw>`

`myoutput.csv` will be populated with the benchmarking data.
