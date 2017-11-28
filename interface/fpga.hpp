#ifndef __FPGA_H_INCLUDED__
#define __FPGA_H_INCLUDED__

#include <iostream>
#include <cstdint>
#include <stdlib.h>
#include <stdexcept>
#include <unistd.h> // usleep

#include "defs.hpp"


class Fpga {
 public:
  Fpga() = default;
  Fpga(int fd, long int g_header, long int buffer0, long int buffer1);
  void run();
  ~Fpga() = default;

 private:
  struct fpga_buffer {
    struct buffer_header head;
    struct chunk data[CHUNKS_PER_BUFFER];
  };

  struct global_header glob_head;
  struct fpga_buffer buf;
  long int dram_bufs[BUFFER_COUNT];
  long int dram_buf_heads[BUFFER_COUNT];
  long int dram_glob_head;
  int dram_fd;

  void read_glob_head();
  void write_glob_head();
  void read_buf();
  void write_result();
};

#endif
