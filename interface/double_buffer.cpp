#include <stdint.h>
#include <cstdlib>
#include <thread>
#include <errno.h>
#include <stdexcept>
#include <stdio.h>

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

DoubleBuffer::DoubleBuffer(){
  glob_head = (global_header *) malloc(BUFFER_SIZE + BUFFER_HEADER_SIZE + GLOBAL_HEADER_SIZE);
  if (!glob_head) {
    throw std::runtime_error("Can't allocate enough memory.");
  }
  // Which buffer to process. 0 -> buffer0 & 1 -> buffer1
  glob_head->active_buf = 0;

  buf_head[0] = (buffer_header *) ((char *) glob_head + GLOBAL_HEADER_SIZE);
  buf_head[1] = (buffer_header *) ((char *) buf_head[0] + BUFFER_HEADER_SIZE + BUFFER_SIZE);

  buf_head[0]->num_chunks = 0;
  buf_head[1]->num_chunks = 0;

  bufs[0] = (struct chunks *) ((char *)buf_head[0] + BUFFER_HEADER_SIZE);
  bufs[1] = (struct chunks *) ((char *)buf_head[1] + BUFFER_HEADER_SIZE);

  chunk_to_write = bufs[0];

  dev_if = DeviceInterface(bufs[0], bufs[1]);
}

struct chunk *DoubleBuffer::get_chunk() {
  uint32_t *n = &buf_head[glob_head->active_buf]->num_chunks;
  if(*n >= CHUNKS_PER_BUFFER){
    return nullptr;
  }
  *n++;
  struct chunk *old_buf_ptr = chunk_to_write;
  // hops to next chunk 64 bytes forward
  chunk_to_write += 1;
  return old_buf_ptr;
}

struct chunk *DoubleBuffer::start_processing() {
  // run kernel
  dev_if->run_fpga(bufs[glob_head->active_buf]);
  // flip buffers
  glob_head->active_buf = 1 - glob_head->active_buf;

  // result is in our new active buffer, which is also where we write new data.
  chunk_to_write = bufs[glob_head->active_buf];
  return chunk_to_write;
}

DoubleBuffer::~DoubleBuffer() {
  free(glob_head);
}
