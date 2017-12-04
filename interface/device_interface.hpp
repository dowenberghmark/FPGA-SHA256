#ifndef __DEVICE_INTERFACES_H_INCLUDED__
#define __DEVICE_INTERFACES_H_INCLUDED__

#include "defs.hpp"
#include "xcl2.hpp"
#include <vector>


class DeviceInterface {
public:
  DeviceInterface() = default;
  DeviceInterface(struct chunk **buffer0, struct chunk **buffer1);
  // result written directly to buf
  struct chunk *run_fpga(int num_chunks, int active_buf);
  void read_last_result(int active_buf);
  void unmap_last_result(int active_buf);
  ~DeviceInterface() = default;

private:
  int first_flag;
  cl::CommandQueue q;
  cl::Buffer ocl_bufs[BUFFER_COUNT];
  cl::Program program;
  cl::Kernel krnl_sha;

  int res_num_chunks[BUFFER_COUNT];
  struct chunk **host_bufs[BUFFER_COUNT];
  cl_mem_ext_ptr_t buffer_ext0, buffer_ext1;  // Declaring two extensions for both buffers
};


#endif
