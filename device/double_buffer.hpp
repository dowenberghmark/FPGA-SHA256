#ifndef DEVICE_DOUBLE_BUFFER_HPP_
#define DEVICE_DOUBLE_BUFFER_HPP_
#include <stdint.h>
#include <cstdlib>

#include "defs.hpp"
#include "device_interface.hpp"
#include "device_information.hpp"


class DoubleBuffer {
 public:
  struct chunk *get_chunk();
  void regret_get_chunk();
  struct buffer start_processing();
  struct buffer get_last_result();
  DoubleBuffer();
  ~DoubleBuffer();
  struct buffer bufs[BUFFER_COUNT];

 private:
  struct chunk *chunk_to_write;  // chunk to write in active buffer
  struct global_header glob_head;
  int second_batch_counter;
  DeviceInfo *dev_info;
  DeviceInterface *dev_if[NUMBER_OF_KERNELS];
  // used to signal that we have to reset num_chunks after a flip
  int flip_flag;
};

#endif  // DEVICE_DOUBLE_BUFFER_HPP_
