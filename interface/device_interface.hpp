#ifndef __DEVICE_INTERFACES_H_INCLUDED__
#define __DEVICE_INTERFACES_H_INCLUDED__

#include "defs.hpp"
#include "xcl2.hpp"
#include <vector>


class DeviceInterface {
public:
  DeviceInterface();
  // result written directly to buf
  struct chunk *get_first_buffer();
  struct chunk *run_fpga(int num_chunks, int active_buf);
  struct chunk *read_last_result(int active_buf);
  void unmap_last_result(int active_buf);
  ~DeviceInterface() = default;

private:
  int first_flag;
  cl::CommandQueue q;
  cl::Buffer ocl_bufs[BUFFER_COUNT];
  cl::Program program;
  cl::Kernel krnl_sha;
  // keep everything in scope
  cl::Context context;
  cl::Program::Binaries bins;
  std::vector<cl::Device> devices;
  cl::Device device;

  int res_num_chunks[BUFFER_COUNT];
  void *host_bufs[BUFFER_COUNT];
  cl_mem_ext_ptr_t buffer_ext0, buffer_ext1;  // Declaring two extensions for both buffers
};


#endif
