#include <stdint.h>
#include <cstdlib>
#include <thread>
#include <errno.h>
#include <stdexcept>
#include <stdio.h>

#include "double_buffer.hpp"
#include "defs.hpp"
#include "fpga.hpp"
#include "utils.hpp"

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

DoubleBuffer::DoubleBuffer(char const *fpga_path) {
  glob_head = (global_header *) malloc(BUFFER_SIZE + BUFFER_HEADER_SIZE + GLOBAL_HEADER_SIZE);
  if (!glob_head) {
    throw std::runtime_error("Can't allocate enough memory.");
  }
  //Signals to the FPGA to start processing when set to 1
  glob_head->start_proc = 0;
  // Which buffer to process. 0 -> buffer0 & 1 -> buffer1
  glob_head->active_buf = 1;

  buf_head = (buffer_header *) ((char *) glob_head + GLOBAL_HEADER_SIZE);

  buf_head->num_chunks = 0;
  if (MODE == LOCAL) {
    buf_head->rdy_flag = 0;
  } else if (MODE == AWS) {
    // Ready flag 1 right now since the fpga is not implemented.
    buf_head->rdy_flag = 1;
  }

  buf = (char *) buf_head + BUFFER_HEADER_SIZE;

  chunk_to_write = buf;
  chunk_counter = 0;

  dram_glob_head = OFFSET_IN_FPGA_DRAM;
  dram_buf_heads[0] = OFFSET_IN_FPGA_DRAM + GLOBAL_HEADER_SIZE;
  dram_buf_heads[1] = dram_buf_heads[0] + BUFFER_HEADER_SIZE + BUFFER_SIZE;

  dram_bufs[0] = dram_buf_heads[0] + BUFFER_HEADER_SIZE;
  dram_bufs[1] = dram_buf_heads[1] + BUFFER_HEADER_SIZE;

  dram_fd = open_file(fpga_path);
  if (dram_fd < 0) {
    throw std::runtime_error("Can't open fpga fd.");
  }

  if (MODE == LOCAL) {
    fpga = Fpga(dram_fd, dram_glob_head, dram_buf_heads[0], dram_buf_heads[1]);
    t = std::thread(&Fpga::run, fpga);
  }
}

char *DoubleBuffer::get_chunk() {
  if (chunk_counter >= CHUNKS_PER_BUFFER) {
    return nullptr;
  }
  chunk_counter++;
  char *old_buf_ptr = chunk_to_write;
  chunk_to_write += CHUNK_SIZE;
  return old_buf_ptr;
}

void DoubleBuffer::start_processing() {
  int ret;

  glob_head->active_buf = 1 - glob_head->active_buf;
  buf_head->num_chunks = chunk_counter;

  ret = pwrite_all(dram_fd, buf_head, BUFFER_HEADER_SIZE + BUFFER_SIZE, dram_buf_heads[glob_head->active_buf]);
  if (ret < 0) {
    throw std::runtime_error("Can't write to dram and start fpga.");
  }

  chunk_to_write = buf;
  chunk_counter = 0;
  glob_head->start_proc = 1;
  // active_buffer is hopefully written before start_processing.
  // keep in mind if we get weird behavior from fpga.
  ret = pwrite_all(dram_fd, glob_head, GLOBAL_HEADER_SIZE, dram_glob_head);
  if (ret < 0) {
    throw std::runtime_error("Can't write to dram and start fpga.");
  }
}

char *DoubleBuffer::get_result() {
  int ret;
  int buf_i = glob_head->active_buf;

  // wait for ready flag
  do {
    ret = pread_all(dram_fd, buf_head, BUFFER_HEADER_SIZE, dram_buf_heads[buf_i]);
    if (ret < 0) return nullptr;

  } while (buf_head->rdy_flag == 0);

  // reset ready flag
  if (MODE == LOCAL) {
    buf_head->rdy_flag = 0;
  } else if (MODE == AWS) {
    // Ready flag 1 right now since the fpga is not implemented.
    buf_head->rdy_flag = 1;
  }

  ret = pwrite_all(dram_fd, buf_head, BUFFER_HEADER_SIZE, dram_buf_heads[buf_i]);
  if (ret < 0) return nullptr;

  // get result
  ret = pread_all(dram_fd, buf, CHUNK_SIZE*buf_head->num_chunks, dram_bufs[buf_i]);
  if (ret < 0) return nullptr;

  return buf;
}

void DoubleBuffer::done() {
  int ret;
  glob_head->start_proc = 1337;
  ret = pwrite_all(dram_fd, glob_head, GLOBAL_HEADER_SIZE, dram_glob_head);
  if (ret < 0) {
    throw std::runtime_error("Can't write to dram and stop fpga.");
  }

  if (MODE == LOCAL) {
    t.join();
  }

  close_file(dram_fd);
}

DoubleBuffer::~DoubleBuffer() {
  free(glob_head);
}
