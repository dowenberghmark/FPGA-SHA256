#include <stdio.h>

#include "fpga.hpp"
#include "utils.hpp"


Fpga::Fpga(int fd, long int glob_head, long int buf0, long int buf1) {
  // set dram addresses.
  dram_fd = fd;
  dram_buf_heads[0] = buf0;
  dram_buf_heads[1] = buf1;
  dram_bufs[0] = dram_buf_heads[0] + BUFFER_HEADER_SIZE;
  dram_bufs[1] = dram_buf_heads[1] + BUFFER_HEADER_SIZE;
  dram_glob_head = glob_head;
}

// run this function as a thread. communicate through the shared file.
void Fpga::run() {
  // dummy fpga loop
  while (1) {
    do {
      read_glob_head();
      usleep(5000);
    } while (!glob_head.start_proc);

    usleep(500000);
    // only for testing so we can quit nicely
    if (glob_head.start_proc == 1337) {
      break;
    }
    // restore start proc flag.
    glob_head.start_proc = 0;
    write_glob_head();

    read_buf();
    // do dummy sha-256 to received chunks
    struct chunk *chunks = buf.data;
    for (size_t i = 0; i < buf.head.num_chunks; i++) {
      for (int j = 0; j < 32; j++) {
	chunks[i].data[j] = i;
      }
      std::cout << "result written to chunk: " << i << "\n";
    }
    std::cout << "buffer" << glob_head.active_buf << " processed, num_chunks: " << buf.head.num_chunks << " rdy_flag: " << buf.head.rdy_flag << "\n";

    // write back result and ready flag
    buf.head.rdy_flag = 1;
    write_result();
  }
  std::cout << "quitting fpga run" << "\n";
}

void Fpga::read_glob_head() {
  int ret;
  ret = pread_all(dram_fd, &glob_head, GLOBAL_HEADER_SIZE, dram_glob_head);
  if (ret < 0) {
    throw std::runtime_error("Can't write to dram from fpga.");
  }
}

void Fpga::write_glob_head() {
  int ret;
  ret = pwrite_all(dram_fd, &glob_head, GLOBAL_HEADER_SIZE, dram_glob_head);
  if (ret < 0) {
    throw std::runtime_error("Can't write to dram from fpga.");
  }
}

void Fpga::read_buf() {
  int ret;
  ret = pread_all(dram_fd, &buf.head, BUFFER_HEADER_SIZE, dram_buf_heads[glob_head.active_buf]);
  if (ret < -1) {
    throw std::runtime_error("Can't write to dram from fpga.");
  }
  // only read the chunks that have been written to.
  ret = pread_all(dram_fd, &buf.data, CHUNK_SIZE*buf.head.num_chunks, dram_bufs[glob_head.active_buf]);
  if (ret < -1) {
    throw std::runtime_error("Can't write to dram from fpga.");
  }
}

void Fpga::write_result() {
  int ret;
  ret = pwrite_all(dram_fd, &buf.head, BUFFER_HEADER_SIZE+CHUNK_SIZE*buf.head.num_chunks, dram_buf_heads[glob_head.active_buf]);
  if (ret < -1) {
    throw std::runtime_error("Can't write to dram from fpga.");
  }
}
