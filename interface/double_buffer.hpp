#ifndef __DOUBLE_BUFFER_H__
#define  __DOUBLE_BUFFER_H__
#include <stdint.h>
#include <cstdlib>

#include "defs.hpp"
#include "device_interface.hpp"


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
  struct chunk *chunk_to_write; // chunk to write in active buffer
  struct global_header glob_head;

  DeviceInterface *dev_if;
  // used to signal that we have to reset num_chunks after a flip
  int flip_flag;
};

#endif
