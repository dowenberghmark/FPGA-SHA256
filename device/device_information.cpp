#include "xcl2.hpp"
#include "device_information.hpp"


DeviceInfo::DeviceInfo() {
  // The get_xil_devices will return vector of Xilinx Devices
  std::vector<cl::Device> devices = xcl::get_xil_devices();
  device = devices[0];

  //Creating Context and Command Queue for selected Device
  context = cl::Context(device);

  std::string device_name = device.getInfo<CL_DEVICE_NAME>();
  std::cout << "Found Device=" << device_name.c_str() << std::endl;

  // import_binary() command will find the OpenCL binary file created using the
  // xocc compiler load into OpenCL Binary and return as Binaries
  // OpenCL and it can contain many functions which can be executed on the
  // device.
  std::string binaryFile = xcl::find_binary_file(device_name, "device_kernel");
  cl::Program::Binaries bins = xcl::import_binary_file(binaryFile);
  devices.resize(1);
  program = cl::Program(context, devices, bins);

}
