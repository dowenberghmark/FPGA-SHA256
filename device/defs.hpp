#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <cstdint>

#define BUFFER_HEADER_SIZE sizeof(struct buffer_header)
#define GLOBAL_HEADER_SIZE sizeof(struct global_header)
#define CHUNK_SIZE sizeof(struct chunk)
#define CHUNKS_PER_BUFFER 3
#define BUFFER_SIZE (CHUNKS_PER_BUFFER * CHUNK_SIZE)


const int BUFFER_COUNT = 2;

struct chunk {
  char data[64];
};

struct buffer {
  int num_chunks;
  struct chunk *chunks[2];
};

struct global_header {
  int active_buf;
};

#endif
