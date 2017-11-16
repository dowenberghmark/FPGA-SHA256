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
  Fpga(void *g_header, void *buffer0, void *buffer1);
  void run();
  ~Fpga() = default;

private:
  struct global_header *global_header;
  struct buffer *buffers[2];

};

#endif
