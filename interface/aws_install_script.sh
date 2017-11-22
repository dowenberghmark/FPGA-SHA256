#!/usr/bin/env bash
# Loads the CL_DRAM_DMA CustomLogic Example. The file works manually at least.

cd $AWS_FPGA_REPO_DIR
source sdk_setup.sh
sudo fpga-clear-local-image  -S 0
sudo fpga-load-local-image -S 0 -I agfi-08f98fa67671454fe
sudo yum groupinstall "Development tools"
sudo yum install kernel-devel
cd sdk/linux_kernel_drivers/edma
make
echo 'edma' | sudo tee --append /etc/modules-load.d/edma.conf
sudo cp edma-drv.ko /lib/modules/`uname -r`/
sudo depmod
sudo modprobe edma-drv
cd
git clone https://github.com/dowenberghmark/PineappleExpress.git
