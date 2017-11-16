#ifndef __DOUBLE_BUFFER_H__
#define  __DOUBLE_BUFFER_H__
#include "defs.hpp"
#include <stdint.h>
#include <cstdlib>

class Double_buffer
{
 private:
  char *first_buffer; //start of first buffer
  char *second_buffer; //start of second buffer
  char *place_to_write; //place to write to in active buffer
  void *global_start_of_buffer;

  global_header *glob_head;
  buffer_header *buf_head_first;
  buffer_header *buf_head_second;
  // buffer_header buf_head[2];
  //global_header global_head;
  uint32_t chunk_counter;
  uint32_t max_chunks;
  uint32_t active_buffer;
 public:
  char *get_chunk();
  void start_processing();
  char *get_result();
  Double_buffer(uint32_t chunks);
  ~Double_buffer() = default;


};

#endif
