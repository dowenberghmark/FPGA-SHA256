#ifndef DEVICE_DEFS_HPP_
#define DEVICE_DEFS_HPP_

#include <cstdint>

#define BUFFER_HEADER_SIZE sizeof(struct buffer_header)
#define GLOBAL_HEADER_SIZE sizeof(struct global_header)
#define CHUNK_SIZE sizeof(struct chunk)

// defined in main_cpu.cpp
extern size_t BUFFER_SIZE;
extern int CHUNKS_PER_BUFFER;

const int BUFFER_COUNT = 1;

struct chunk {
  char data[64];
};

struct buffer {
  int num_chunks;
  struct chunk *chunks;
};

struct global_header {
  int active_buf;
};

#endif  // DEVICE_DEFS_HPP_
