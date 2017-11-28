#ifndef __DEVICE_INTERFACES_H_INCLUDED__
#define __DEVICE_INTERFACES_H_INCLUDED__

#include "defs.hpp"

class DeviceInterface {
public:
  DeviceInterface(struct chunk *buffer0, struct chunk *buffer1);
  // result written directly to buf
  void run_fpga(struct buffer *buf);
  ~DeviceInterface() = default;

private:
  int first_flag;
  cl::CommandQueue queue;
  cl::Buffer ocl_bufs[BUFFER_COUNT];
  struct chunk host_bufs[BUFFER_COUNT];
};


#endif
