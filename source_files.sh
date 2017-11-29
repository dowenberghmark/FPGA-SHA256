#!/usr/bin/env bash
p=$(pwd)
cd /home/centos/aws-fpga
source hdk_setup.sh
source sdaccel_setup.sh
source /opt/Xilinx/SDx/2017.1.op/settings64.sh
cd $p
source vendor_exports.sh
