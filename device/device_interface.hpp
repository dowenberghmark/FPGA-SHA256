#ifndef __DEVICE_INTERFACES_H_INCLUDED__
#define __DEVICE_INTERFACES_H_INCLUDED__

#include "defs.hpp"
#include "xcl2.hpp"
#include <vector>
#include "device_information.hpp"

class DeviceInterface {
 public:
  DeviceInterface(DeviceInfo information, const char *kernel_name, int banks);
  struct chunk *run_fpga(int num_chunks, int active_buf);
  struct chunk *read_last_result(int active_buf);
  struct chunk *fetch_buffer(int active_buf);
  void unmap_last_result(int active_buf);
  ~DeviceInterface() = default;

 private:
  cl::CommandQueue q;
  cl::Program program;
  cl::Kernel krnl_sha;

  cl::Buffer ocl_bufs[BUFFER_COUNT];
  void *host_bufs[BUFFER_COUNT];
  cl_mem_ext_ptr_t buffer_ext[BUFFER_COUNT];
};

#endif
