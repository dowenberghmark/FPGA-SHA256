#!/usr/bin/env bash
# Loads the CL_DRAM_DMA CustomLogic Example. Interactive.

# What this script does:
## sets up an fpga: https://github.com/aws/aws-fpga/blob/master/hdk/cl/examples/README.md

## loads this example: https://github.com/aws/aws-fpga/tree/master/hdk/cl/examples/cl_dram_dma

## installs edma:
## https://github.com/aws/aws-fpga/blob/master/sdk/linux_kernel_drivers/edma/edma_install.md


cd $AWS_FPGA_REPO_DIR
source sdk_setup.sh
sudo fpga-clear-local-image  -S 0
sudo fpga-load-local-image -S 0 -I agfi-08f98fa67671454fe
sudo yum -y groupinstall "Development tools"
sudo yum -y install kernel-devel
cd sdk/linux_kernel_drivers/edma
make
echo 'edma' | sudo tee --append /etc/modules-load.d/edma.conf
sudo cp edma-drv.ko /lib/modules/`uname -r`/
sudo depmod
sudo modprobe edma-drv
