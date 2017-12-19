// device interface for connecting cpu to fpga using opencl code.
// initialization based on SDaccel hello world program
#include "device_information.hpp"

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

DeviceInterface::DeviceInterface(DeviceInfo *information, const char *kernel_name, int banks) {
  // This call will extract a kernel out of the program we loaded in the
  // previous line. A kernel is an OpenCL function that is executed on the
  // FPGA. This function is defined in the device/device_kernel.cl file.

  q = cl::CommandQueue(information->context, information->device, CL_QUEUE_PROFILING_ENABLE);
  krnl_sha = cl::Kernel(information->program, kernel_name);

  unsigned xcl_banks[4] = {
    XCL_MEM_DDR_BANK0,
    XCL_MEM_DDR_BANK1,
    XCL_MEM_DDR_BANK2,
    XCL_MEM_DDR_BANK3
  };

  for (int i = 0; i < BUFFER_COUNT; i++) {
    buffer_ext[i].flags = xcl_banks[i + banks];
    buffer_ext[i].obj = NULL;
    buffer_ext[i].param = 0;
  }

  // first half of buffers are only read by kernel
  // second half of buffers should only be written by kernel (not atm)
  int err;
  unsigned flags = CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX;
  for (int i = 0; i < BUFFER_COUNT; i++) {
    ocl_bufs[i] = cl::Buffer(information->context, flags, BUFFER_SIZE, &buffer_ext[i], &err);
    if (err != CL_SUCCESS) {
      printf("Error: Failed to allocate buffer in DDR bank %d %zu\n", i, BUFFER_SIZE);
    }
  }

  for (int i = 0; i < BUFFER_COUNT; i++) {
    host_bufs[i] = nullptr;
  }

  host_bufs[0] = (struct chunk *) q.enqueueMapBuffer(ocl_bufs[0], CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, BUFFER_SIZE, NULL, NULL, &err);
}

// used to bootstrap
struct chunk *DeviceInterface::fetch_buffer(int active_buf) {
  return (struct chunk *) host_bufs[active_buf];
}

struct chunk *DeviceInterface::run_fpga(int num_chunks, int active_buf) {
  cl_int err;
  q.enqueueUnmapMemObject(ocl_bufs[active_buf], host_bufs[active_buf], NULL, NULL);
  q.finish();

  // set the kernel Arguments
  int narg = 0;
  krnl_sha.setArg(narg++, ocl_bufs[0]);
  krnl_sha.setArg(narg++, ocl_bufs[1]);
  krnl_sha.setArg(narg++, num_chunks);
  krnl_sha.setArg(narg++, active_buf);

  // launch the Kernel
  q.enqueueTask(krnl_sha);

  // previous computations result
  host_bufs[1 - active_buf] = (struct chunk *) q.enqueueMapBuffer(ocl_bufs[1 - active_buf], CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, BUFFER_SIZE, NULL, NULL, &err);
  check_error(err);

  return (struct chunk *) host_bufs[1 - active_buf];
}

struct chunk *DeviceInterface::read_last_result(int active_buf) {
  int err;
  q.enqueueUnmapMemObject(ocl_bufs[active_buf], host_bufs[active_buf], NULL, NULL);
  host_bufs[1 - active_buf] = q.enqueueMapBuffer(ocl_bufs[1 - active_buf], CL_TRUE, CL_MAP_READ | CL_MAP_WRITE, 0, BUFFER_SIZE, NULL, NULL, &err);
  check_error(err);
  return (struct chunk *) host_bufs[1 - active_buf];
}

void DeviceInterface::unmap_last_result(int active_buf) {
  q.enqueueUnmapMemObject(ocl_bufs[1 - active_buf], host_bufs[1 - active_buf]);
  q.finish();
}
