#ifndef DEVICE_DEVICE_INTERFACE_HPP_
#define DEVICE_DEVICE_INTERFACE_HPP_

#include <vector>
#include "defs.hpp"
#include "xcl2.hpp"


class DeviceInterface {
 public:
  DeviceInterface();
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

#endif  // DEVICE_DEVICE_INTERFACE_HPP_
