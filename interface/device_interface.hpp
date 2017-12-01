#ifndef __DEVICE_INTERFACES_H_INCLUDED__
#define __DEVICE_INTERFACES_H_INCLUDED__

#include "defs.hpp"
#include "xcl2.hpp"
#include <vector>


class DeviceInterface {
public:
  DeviceInterface() = default;
  DeviceInterface(struct buffer **bufs);
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
  struct buffer **host_bufs;
};


#endif
