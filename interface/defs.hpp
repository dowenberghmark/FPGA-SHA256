#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <cstdint>

#define BUFFER_HEADER_SIZE sizeof(struct buffer_header)
#define GLOBAL_HEADER_SIZE sizeof(struct global_header)
#define CHUNK_SIZE sizeof(struct chunk)
#define CHUNKS_PER_BUFFER 4
#define BUFFER_SIZE (CHUNKS_PER_BUFFER * CHUNK_SIZE)


const int BUFFER_COUNT = 4;

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

// int modulo1234(int x, int N) {
//   return (x % N + N) % N;
// }
//
// int PREV(int x) {
//   return modulo1234(x-1, BUFFER_COUNT);
// }
//
// int NEXT(int x) {
//   return modulo1234(x+1, BUFFER_COUNT);
// }


#endif
