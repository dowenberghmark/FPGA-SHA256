// device interface for connecting cpu to fpga using opencl code.
// initialization based on SDaccel hello world program

#include "device_interface.hpp"
#include "defs.hpp"
#include "xcl2.hpp"


DeviceInterface::DeviceInterface(struct chunk *buffer0, struct chunk *buffer1) {
  first_flag = 1;
  // The get_xil_devices will return vector of Xilinx Devices
  std::vector<cl::Device> devices = xcl::get_xil_devices();
  cl::Device device = devices[0];

  //Creating Context and Command Queue for selected Device
  cl::Context context(device);
  q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE);
  std::string device_name = device.getInfo<CL_DEVICE_NAME>();
  std::cout << "Found Device=" << device_name.c_str() << std::endl;

  // import_binary() command will find the OpenCL binary file created using the
  // xocc compiler load into OpenCL Binary and return as Binaries
  // OpenCL and it can contain many functions which can be executed on the
  // device.
  std::string binaryFile = xcl::find_binary_file(device_name,"device_kernel");
  cl::Program::Binaries bins = xcl::import_binary_file(binaryFile);
  devices.resize(1);
  cl::Program program(context, devices, bins);

  ocl_bufs[0] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, BUFFER_SIZE, buffer0);
  ocl_bufs[1] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE, BUFFER_SIZE, buffer1);

  host_bufs[0].chunks = buffer0;
  host_bufs[1].chunks = buffer1;

  // This call will extract a kernel out of the program we loaded in the
  // previous line. A kernel is an OpenCL function that is executed on the
  // FPGA. This function is defined in the interface/device_kernel.cl file.
  krnl_sha = cl::Kernel(program, "fpga_sha");
}

void DeviceInterface::run_fpga(int num_chunks, int active_buf) {
  // The data will be be transferred from system memory over PCIe to the FPGA on-board
  // DDR memory. blocking.
  q.enqueueWriteBuffer(ocl_bufs[active_buf], CL_TRUE, 0, num_chunks*CHUNK_SIZE, host_bufs[active_buf].chunks, NULL, NULL);
  host_bufs[active_buf].num_chunks = num_chunks;

  //set the kernel Arguments
  int narg=0;
  krnl_sha.setArg(narg++, ocl_bufs[active_buf]);
  krnl_sha.setArg(narg++, num_chunks);

  //Launch the Kernel
  q.enqueueTask(krnl_sha);

  // The result of the previous kernel execution will need to be retrieved in
  // order to view the results. This call will write the data from the
  // buffer_result cl_mem object to the source_results vector
  // first_flag causes us to not read result buffer first time
  if (!first_flag) {
    // blocking.
    q.enqueueReadBuffer(ocl_bufs[1 - active_buf], CL_TRUE, 0, host_bufs[1 - active_buf].num_chunks*CHUNK_SIZE, host_bufs[1 - active_buf].chunks, NULL, NULL);
    host_bufs[1 - active_buf].num_chunks = 0;
  } else {
    first_flag = 0;
  }
  q.finish();
}
