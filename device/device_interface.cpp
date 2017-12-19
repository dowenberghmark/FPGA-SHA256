// device interface for connecting cpu to fpga using opencl code.
// initialization based on SDaccel hello world program

#include <string>
#include <vector>

#include "device_interface.hpp"
#include "defs.hpp"
#include "xcl2.hpp"


void check_error(cl_int err) {
  if (err) {
    printf("ERROR: Operation Failed: %d\n", err);
    exit(EXIT_FAILURE);
  }
}

DeviceInterface::DeviceInterface() {
  first_flag = 1;
  // The get_xil_devices will return vector of Xilinx Devices
  std::vector<cl::Device> devices = xcl::get_xil_devices();
  cl::Device device = devices[0];

  // Creating Context and Command Queue for selected Device
  cl::Context context(device);
  q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE);
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
  // This call will extract a kernel out of the program we loaded in the
  // previous line. A kernel is an OpenCL function that is executed on the
  // FPGA. This function is defined in the device/device_kernel.cl file.
  krnl_sha = cl::Kernel(program, "hashing_kernel");

  unsigned xcl_banks[4] = {
    XCL_MEM_DDR_BANK0,
    XCL_MEM_DDR_BANK1,
    XCL_MEM_DDR_BANK2,
    XCL_MEM_DDR_BANK3
  };

    for (int i = 0; i < BUFFER_COUNT; i++) {
    host_bufs[i] = (struct chunk *) aligned_alloc(4096, BUFFER_SIZE);
  }

  for (int i = 0; i < BUFFER_COUNT; i++) {
    buffer_ext[i].flags = xcl_banks[i];
    buffer_ext[i].obj = NULL;
    buffer_ext[i].param = 0;
  }
//   host_bufs[0] = (struct chunk *) q.enqueueMapBuffer(ocl_bufs[0], CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, BUFFER_SIZE, NULL, NULL, &err);
// }



  // first half of buffers are only read by kernel
  // second half of buffers should only be written by kernel (not atm)
  int err;
  unsigned flags = CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR;
  for (int i = 0; i < BUFFER_COUNT; i++) {
    ocl_bufs[i] = cl::Buffer(context, flags, BUFFER_SIZE, host_bufs[i], &err);
    if (err != CL_SUCCESS) {
      printf("Error: Failed to allocate buffer in DDR bank %d %zu\n", i, BUFFER_SIZE);
    }
  }

}

// used to bootstrap
struct chunk *DeviceInterface::fetch_buffer(int active_buf) {
  return (struct chunk *) host_bufs[active_buf];
}


struct chunk *DeviceInterface::run_fpga(int num_chunks, int active_buf) {
  cl_int err;
  mem_bufs[active_buf].push_back(ocl_bufs[active_buf]);
  mem_bufs[1 - active_buf].push_back(ocl_bufs[1-active_buf]);

  q.enqueueMigrateMemObjects(mem_bufs[active_buf], 0);
  //  q.finish();
  //  q.finish();

  // set the kernel Arguments
  int narg = 0;
  krnl_sha.setArg(narg++, ocl_bufs[0]);
  krnl_sha.setArg(narg++, ocl_bufs[1]);
  krnl_sha.setArg(narg++, num_chunks);
  krnl_sha.setArg(narg++, active_buf);

  // launch the Kernel
  q.enqueueTask(krnl_sha);

  // previous computations result
 if (!first_flag){
    q.enqueueMigrateMemObjects(mem_bufs[1-active_buf], CL_MIGRATE_MEM_OBJECT_HOST);
  } else {
   //   mem_bufs[1].pop_back();
    first_flag = 0;
  }
  q.finish();
  return (struct chunk *) host_bufs[1 - active_buf];
}

 struct chunk *DeviceInterface::read_last_result(int active_buf, int num_chunks) {
  // int err;
  // q.enqueueUnmapMemObject(ocl_bufs[active_buf], host_bufs[active_buf], NULL, NULL);

  // host_bufs[1 - active_buf] = q.enqueueMapBuffer(ocl_bufs[1 - active_buf], CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, BUFFER_SIZE, NULL, NULL, &err);
  // check_error(err);

  q.enqueueMigrateMemObjects(mem_bufs[1-active_buf], CL_MIGRATE_MEM_OBJECT_HOST);
  q.finish();
  //  clReleaseProgram();
  return (struct chunk *) host_bufs[1 - active_buf];
}

void DeviceInterface::unmap_last_result(int active_buf) {
  q.enqueueUnmapMemObject(ocl_bufs[1 - active_buf], host_bufs[1 - active_buf]);
  q.finish();
}
