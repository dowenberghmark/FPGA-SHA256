#ifndef __DEVICE_INFO_H_INCLUDED__
#define __DEVICE_INFO_H_INCLUDED__

#include "xcl2.hpp"

class DeviceInfo
{
 public:
  DeviceInfo();
  ~DeviceInfo() = default;
  cl::Device device;
  cl::Context context;
  cl::Program program;
  

};
#endif
