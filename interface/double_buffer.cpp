#include <stdint.h>
#include <cstdlib>
#include <thread>
#include <errno.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

#include "double_buffer.hpp"
#include "defs.hpp"

/**
   The system has three buffers in total. CPU/API shares one buffer, denoted here as CPU-buffer.
   FPGA has two buffers in DRAM, dram-buffer0 and dram-buffer1. The dummy version has its DRAM
   in the file dram.hex and also an extra buffer to load data from the DRAM.

   Buffer cycle:
   1. CPU fills up CPU-buffer chunk-wise.
   2. CPU calls start_processing and CPU-buffer is copied to dram-buffer0.
   3. Interface retrieves result from dram-buffer1 and loads it into CPU-buffer.
   4. CPU calls get_result and receives result from interface.
   5. CPU fills up CPU-buffer chunk-wise.
   6. CPU calls start_processing and CPU-buffer is copied to dram-buffer1.
   7. Interface retrieves result from dram-buffer0 and loads it into CPU-buffer.
   8. CPU calls get_result and receives result from interface.
   9. goto 1.
 */


DoubleBuffer::DoubleBuffer() {
  glob_head.active_buf = 0;
  for (int i = 0; i < BUFFER_COUNT; i++) {
    bufs[i].num_chunks = 0;
  }

  dev_if = new DeviceInterface();
  bufs[glob_head.active_buf].chunks = dev_if->fetch_buffer(glob_head.active_buf);
  chunk_to_write = bufs[glob_head.active_buf].chunks;
  flip_flag = 1;
}

struct chunk *DoubleBuffer::get_chunk() {
  if (flip_flag) {
    bufs[glob_head.active_buf].num_chunks = 0;
    flip_flag = 0;
  }

  if (bufs[glob_head.active_buf].num_chunks >= CHUNKS_PER_BUFFER) {
    return nullptr;
  }

  bufs[glob_head.active_buf].num_chunks++;
  struct chunk *old_buf_ptr = chunk_to_write;
  // hops to next chunk 64 bytes forward
  chunk_to_write += 1;
  return old_buf_ptr;
}

struct buffer DoubleBuffer::start_processing() {
  // run kernel
  bufs[1 - glob_head.active_buf].chunks = dev_if->run_fpga(bufs[glob_head.active_buf].num_chunks, glob_head.active_buf);
  // flip buffers
  glob_head.active_buf = 1 - glob_head.active_buf;
  flip_flag = 1;
  // result is in our new active buffer, which is also where we write new data.
  chunk_to_write = bufs[glob_head.active_buf].chunks;
  return bufs[glob_head.active_buf];
}

struct buffer DoubleBuffer::get_last_result() {
  bufs[1 - glob_head.active_buf].chunks = dev_if->read_last_result(glob_head.active_buf);
  return bufs[1 - glob_head.active_buf];
}

void DoubleBuffer::regret_get_chunk() {
  bufs[glob_head.active_buf].num_chunks--;
  chunk_to_write -= 1;
}

DoubleBuffer::~DoubleBuffer() {
  dev_if->unmap_last_result(glob_head.active_buf);
  delete dev_if;
}
