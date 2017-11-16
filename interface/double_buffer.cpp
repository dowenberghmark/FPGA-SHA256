#include <stdint.h>
#include <cstdlib>

#include "double_buffer.hpp"
#include "defs.hpp"


Double_buffer::Double_buffer(uint32_t chunks){
  global_start_of_buffer = malloc((chunks * sizeof(chunk) * 2) + 2 * sizeof(buffer_header) + sizeof(global_header));

  glob_head = (global_header *) (global_start_of_buffer);
  glob_head -> start_processing_flag = 0;
  glob_head -> active_buffer_flag = 0;

  buf_head_first = (buffer_header *) (glob_head + sizeof(global_header));
  buf_head_first -> num_chunks = 0;
  buf_head_first -> ready_flag = 0;

  buf_head_second = (buffer_header *) (buf_head_first + sizeof(buffer_header) + sizeof(chunk) * chunks);
  buf_head_second -> num_chunks = 0;
  buf_head_second -> ready_flag = 0;

  first_buffer = (char *) (buf_head_first + sizeof(buffer_header));
  second_buffer = (char *) (buf_head_second + sizeof(buffer_header));
  place_to_write = first_buffer;

  chunk_counter = 0;
Should be fine otherwise
￼	￼ Reply…
  max_chunks = chunks;
  active_buffer = 0;

}

char *Double_buffer::get_chunk(){
  if(chunk_counter == max_chunks){
    return nullptr;
  }
  chunk_counter++;
  char *old_buf_ptr = place_to_write;
  place_to_write += sizeof(chunk);
  return old_buf_ptr;
}


void Double_buffer::start_processing(){
  glob_head -> active_buffer_flag = 1 - glob_head -> active_buffer_flag;
  glob_head -> start_processing_flag = 1;

  if(glob_head -> active_buffer_flag == 0){
    place_to_write = second_buffer;
  }else{
    place_to_write = first_buffer;
  }
}


char *Double_buffer::get_result(){
  char *res_ptr = nullptr;
  if(glob_head -> active_buffer_flag == 0){
    res_ptr = first_buffer;
  }else{
    res_ptr = second_buffer;
  }
    return res_ptr;
}

Double_buffer::~Double_buffer(){
  free(global_start_of_buffer);
}
