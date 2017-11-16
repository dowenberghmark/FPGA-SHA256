#ifndef __DOUBLE_BUFFER_H__
#define  __DOUBLE_BUFFER_H__
#include "defs.hpp"
#include <stdint.h>
#include <cstdlib>
#include "fpga.hpp"

class Double_buffer
{
 private:
  char *first_buf; //start of first buffer
  char *second_buf; //start of second buffer
  char *chunk_to_write; //place to write to in active buffer
  char *global_start_of_buffer;
  //dummy variables
  Fpga fpga;
  std::thread t;

  global_header *glob_head;
  buffer_header *first_buf_head;
  buffer_header *second_buf_head;
  uint32_t chunk_counter;
  uint32_t max_chunks;
 public:
  char *get_chunk();
  void start_processing();
  char *get_result();
  void done();

  Double_buffer(uint32_t chunks);
  ~Double_buffer();

};

#endif
