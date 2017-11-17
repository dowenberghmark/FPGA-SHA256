#include <stdint.h>
#include <cstdlib>
#include <thread>

#include <stdio.h>

#include "double_buffer.hpp"
#include "defs.hpp"
#include "fpga.hpp"


Double_buffer::Double_buffer(uint32_t chunks){
  global_start_of_buffer = (char *) malloc((chunks * CHUNK_SIZE * 2) + (2 * BUFFER_HEADER_SIZE) + GLOBAL_HEADER_SIZE);

  glob_head = (global_header *) (global_start_of_buffer);
  glob_head -> start_processing_flag = 0;
  glob_head -> active_buffer_flag = 1;

  buffer_heads[0] = (buffer_header *) ((char *) glob_head + GLOBAL_HEADER_SIZE);
  buffer_heads[1] = (buffer_header *) ((char *) buffer_heads[0] + BUFFER_HEADER_SIZE + (CHUNK_SIZE * chunks));

  for (int i = 0; i < BUFFER_COUNT; i++) {
    buffer_heads[i] -> num_chunks = 0;
    buffer_heads[i] -> ready_flag = 0;
    buffers[i] = (char *) buffer_heads[i] + BUFFER_HEADER_SIZE;
  }

  chunk_to_write = buffers[0];

  chunk_counter = 0;
  max_chunks = chunks;
  fpga =  Fpga(glob_head, buffer_heads[0], buffer_heads[1]);
  t = std::thread(&Fpga::run, fpga);

}

char *Double_buffer::get_chunk(){
  if(chunk_counter == max_chunks){
    return nullptr;
  }
  chunk_counter++;
  char *old_buf_ptr = chunk_to_write;
  chunk_to_write += CHUNK_SIZE;
  return old_buf_ptr;
}

void Double_buffer::start_processing(){
  glob_head -> active_buffer_flag = 1 - glob_head -> active_buffer_flag;
  if (glob_head -> active_buffer_flag == 0) {
    buffer_heads[0] -> num_chunks = chunk_counter;
    chunk_to_write = buffers[1];
  }else{
    buffer_heads[1] -> num_chunks = chunk_counter;
    chunk_to_write = buffers[0];
  }
  chunk_counter = 0;
  glob_head -> start_processing_flag = 1;
}

char *Double_buffer::get_result(){
  char *res_ptr = nullptr;
  if(glob_head -> active_buffer_flag == 0){
    while(buffer_heads[0] -> ready_flag == 0){}
    buffer_heads[0] -> ready_flag = 0;
    res_ptr = buffers[0];
  }else{
    while(buffer_heads[1] -> ready_flag == 0){}
    buffer_heads[1] -> ready_flag = 0;
    res_ptr = buffers[1];
  }
    return res_ptr;
}

void Double_buffer::done(){

  glob_head->start_processing_flag = 1337;
  t.join();
}

Double_buffer::~Double_buffer(){
  free(global_start_of_buffer);
}
