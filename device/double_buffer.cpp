#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdexcept>

#include "double_buffer.hpp"
#include "defs.hpp"
#include "device_information.hpp"

DoubleBuffer::DoubleBuffer() {
  glob_head.active_buf = 0;
  for (int i = 0; i < BUFFER_COUNT; i++) {
    bufs[i].num_chunks = 0;
  }
  const char *krnl_name_0 = "hashing_kernel0";
  const char *krnl_name_1 = "hashing_kernel1";

  dev_info = new DeviceInfo();
  dev_if[0] = new DeviceInterface(dev_info, krnl_name_0, 0);
  dev_if[1] = new DeviceInterface(dev_info, krnl_name_1, 2);

  bufs[glob_head.active_buf].chunks[0] = dev_if[0]->fetch_buffer(glob_head.active_buf);
  bufs[glob_head.active_buf].chunks[1] = dev_if[1]->fetch_buffer(glob_head.active_buf);
  chunk_to_write = bufs[glob_head.active_buf].chunks[0];

  second_batch_counter = 0;
  flip_flag = 1;
}

struct chunk *DoubleBuffer::get_chunk() {
  if (flip_flag) {
    bufs[glob_head.active_buf].num_chunks = 0;
    flip_flag = 0;
  }
  // illusion of double buffer when we have four buffers,
  if (bufs[glob_head.active_buf].num_chunks == CHUNKS_PER_BUFFER * 2) {
    return nullptr;
  } else if (bufs[glob_head.active_buf].num_chunks == CHUNKS_PER_BUFFER) {
    chunk_to_write = bufs[glob_head.active_buf].chunks[1];
    bufs[1 - glob_head.active_buf].chunks[0] = dev_if[0]->run_fpga(bufs[glob_head.active_buf].num_chunks, glob_head.active_buf);
  }

  bufs[glob_head.active_buf].num_chunks++;
  struct chunk *old_buf_ptr = chunk_to_write;

  // hops to next chunk 64 bytes forward
  chunk_to_write += 1;
  return old_buf_ptr;
}

struct buffer DoubleBuffer::start_processing() {
  // run kernel
  second_batch_counter = bufs[glob_head.active_buf].num_chunks - CHUNKS_PER_BUFFER;
  if (second_batch_counter > 0) {
    bufs[1 - glob_head.active_buf].chunks[1] = dev_if[1]->run_fpga(second_batch_counter, glob_head.active_buf);
  } else {
    bufs[1 - glob_head.active_buf].chunks[1] = dev_if[1]->run_fpga(second_batch_counter, glob_head.active_buf);
    bufs[1 - glob_head.active_buf].chunks[0] = dev_if[0]->run_fpga(bufs[glob_head.active_buf].num_chunks, glob_head.active_buf);
  }
  // flip buffers
  glob_head.active_buf = 1 - glob_head.active_buf;
  flip_flag = 1;
  // result is in our new active buffer, which is also where we write new data.
  chunk_to_write = bufs[glob_head.active_buf].chunks[0];
  return bufs[glob_head.active_buf];
}

struct buffer DoubleBuffer::get_last_result() {
  bufs[1 - glob_head.active_buf].chunks[0] = dev_if[0]->read_last_result(glob_head.active_buf);
  if (bufs[1 - glob_head.active_buf].num_chunks >= CHUNKS_PER_BUFFER) {
     bufs[1 - glob_head.active_buf].chunks[1] = dev_if[1]->read_last_result(glob_head.active_buf);
  }
  return bufs[1 - glob_head.active_buf];
}

void DoubleBuffer::regret_get_chunk() {
  bufs[glob_head.active_buf].num_chunks--;
  chunk_to_write -= 1;
}

DoubleBuffer::~DoubleBuffer() {
  dev_if[0]->unmap_last_result(glob_head.active_buf);
  if (bufs[1 - glob_head.active_buf].num_chunks >= CHUNKS_PER_BUFFER) {
    dev_if[1]->unmap_last_result(glob_head.active_buf);
  } else {
    dev_if[1]->unmap_last_result(1 - glob_head.active_buf);
  }
  delete dev_if[0];
  delete dev_if[1];
  delete dev_info;
}
