#ifndef __DEVICE_INTERFACES_H_INCLUDED__
#define __DEVICE_INTERFACES_H_INCLUDED__

#include "defs.hpp"
#include "xcl2.hpp"
#include <vector>


class DeviceInterface {
public:
  DeviceInterface();
  // result written directly to buf
  struct chunk *get_write_buffer(int active_buf);
  struct chunk *run_fpga(int num_chunks, int active_buf);
  struct chunk *read_last_result(int active_buf);
  void unmap_last_result(int active_buf);
  ~DeviceInterface() = default;

private:
  cl::CommandQueue q;
  cl::Program program;
  cl::Kernel krnl_sha[2];

  cl::Buffer ocl_bufs[BUFFER_COUNT*2];
  void *host_bufs[BUFFER_COUNT*2];
  cl_mem_ext_ptr_t buffer_ext[BUFFER_COUNT*2];  // Declaring two extensions for both buffers

};


#endif
