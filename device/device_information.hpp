#ifndef DEVICE_DEVICE_INFORMATION_HPP_
#define DEVICE_DEVICE_INFORMATION_HPP_
#include "xcl2.hpp"
class DeviceInfo{
 public:
  DeviceInfo();
  ~DeviceInfo() = default;
  cl::Device device;
  cl::Context context;
  cl::Program program;
};
#endif  // DEVICE_DEVICE_INFORMATION_HPP_
