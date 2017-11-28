#ifndef __DOUBLE_BUFFER_H__
#define  __DOUBLE_BUFFER_H__
#include <stdint.h>
#include <cstdlib>

#include "defs.hpp"
#include "device_interface.hpp"


class DoubleBuffer {
 public:
  struct chunk *get_chunk();
  struct chunk *start_processing();

  DoubleBuffer();
  ~DoubleBuffer();

 private:
  struct chunk *chunk_to_write; // chunk to write in active buffer

  struct global_header *glob_head;
  struct buffer_header *buf_head[BUFFER_COUNT];
  struct chunks *bufs[BUFFER_COUNT];

  DeviceInterface dev_if;
};

#endif
