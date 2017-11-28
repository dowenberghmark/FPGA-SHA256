#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <cstdint>
#ifndef MODE
#error MODE compilation flag missing
#endif

enum mode_flags {
   LOCAL = 0,
   AWS = 1
};

#define BUFFER_HEADER_SIZE sizeof(struct buffer_header)
#define GLOBAL_HEADER_SIZE sizeof(struct global_header)
#define CHUNK_SIZE sizeof(struct chunk)
#define CHUNKS_PER_BUFFER 2
#define BUFFER_SIZE (CHUNKS_PER_BUFFER * CHUNK_SIZE)

#define OFFSET_IN_FPGA_DRAM 0 // 0x10000000 in example

const int BUFFER_COUNT = 2;

struct chunk {
  char data[64];
};

struct buffer_header {
  uint32_t num_chunks;
  uint32_t rdy_flag;
};

struct buffer {
  struct buffer_header *buffer_header;
  struct chunk *chunks;
};

struct global_header {
  uint32_t active_buf;
  uint32_t start_proc;
};

#endif
