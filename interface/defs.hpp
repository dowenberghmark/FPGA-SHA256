#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <cstdint>

#define BUFFER_HEADER_SIZE sizeof(struct buffer_header)
#define GLOBAL_HEADER_SIZE sizeof(struct global_header)
#define CHUNK_SIZE sizeof(struct chunk)

const int BUFFER_COUNT = 2;


struct res_chunk{
  char data[32];
};

struct result_struct {
  uint32_t num_chunks;
  res_chunk *res_ptr;
};

struct chunk {
  char data[64];
};

struct buffer_header {
  uint32_t num_chunks;
  uint32_t ready_flag;
};

struct buffer {
  struct buffer_header *buffer_header;
  struct chunk *chunks;
};

struct global_header {
  uint32_t start_processing_flag;
  uint32_t active_buffer_flag;
};

#endif
