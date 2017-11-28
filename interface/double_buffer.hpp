#ifndef __DOUBLE_BUFFER_H__
#define  __DOUBLE_BUFFER_H__
#include <stdint.h>
#include <cstdlib>

#include "defs.hpp"
#include "fpga.hpp"

class DoubleBuffer {
 public:
  char *get_chunk();
  void start_processing();
  char *get_result();
  void done();

  DoubleBuffer(char const *fpga_path);
  ~DoubleBuffer();

 private:
  char *buf; // start of the data in buffers
  char *chunk_to_write; // chunk to write in active buffer
  // dummy variables for 'emulated' fpga thread
  Fpga fpga;
  std::thread t;

  global_header *glob_head;
  buffer_header *buf_head;
  uint32_t chunk_counter;

  int dram_fd;
  // physical addresses to buffer headers in dram
  long int dram_glob_head;
  long int dram_buf_heads[BUFFER_COUNT];
  long int dram_bufs[BUFFER_COUNT];
};

#endif
