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

  first_buf_head = (buffer_header *) ((char *) glob_head + GLOBAL_HEADER_SIZE);
  first_buf_head -> num_chunks = 0;
  first_buf_head -> ready_flag = 0;

  second_buf_head = (buffer_header *) ((char *) first_buf_head + BUFFER_HEADER_SIZE + (CHUNK_SIZE * chunks));
  second_buf_head -> num_chunks = 0;
  second_buf_head -> ready_flag = 0;

  first_buf = (char *) first_buf_head + BUFFER_HEADER_SIZE;
  second_buf = (char *) second_buf_head + BUFFER_HEADER_SIZE;
  chunk_to_write = first_buf;

  chunk_counter = 0;
  max_chunks = chunks;
  fpga =  Fpga(glob_head, first_buf_head, second_buf_head);
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
    first_buf_head -> num_chunks = chunk_counter;
    chunk_to_write = second_buf;
  }else{
    second_buf_head -> num_chunks = chunk_counter;
    chunk_to_write = first_buf;
  }
  chunk_counter = 0;
  glob_head -> start_processing_flag = 1;
}

char *Double_buffer::get_result(){
  char *res_ptr = nullptr;
  if(glob_head -> active_buffer_flag == 0){
    while(first_buf_head -> ready_flag == 0){}
    first_buf_head -> ready_flag = 0;
    res_ptr = first_buf;
  }else{
    while(second_buf_head -> ready_flag == 0){}
    second_buf_head -> ready_flag = 0;
    res_ptr = second_buf;
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
