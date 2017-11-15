#include <iostream>
#include <thread>
#include <string.h>
#include "double_buffer.hpp"

int main(int argc, char *argv[])
{

  char * chunk_placement_ptr;
  Double_buffer* our_double_buffer = new Double_buffer(4);
  const std::string string_array[5] = {"1111111111111111111111111111111111111111111111111111111111111111",
                                    "2222222222222222222222222222222222222222222222222222222222222222",
                                    "3333333333333333333333333333333333333333333333333333333333333333",
                                    "4444444444444444444444444444444444444444444444444444444444444444",
                                    "5555555555555555555555555555555555555555555555555555555555555555"};


  //adding two strings to the buffer and start processing
  // might need to convert it to a bits
  chunk_placement_ptr = our_double_buffer->get_chunk();
  strcpy(chunk_placement_ptr, string_array[0].c_str());

  chunk_placement_ptr = our_double_buffer->get_chunk();
  strcpy(chunk_placement_ptr, string_array[1].c_str());

  our_double_buffer->start_processing();
  //This depends on how we return the result
  chunk_placement_ptr = our_double_buffer->get_result();

  //adding two strings to the buffer and start processing
  // might need to convert it to a bits
  chunk_placement_ptr = our_double_buffer->get_chunk();
  strcpy(chunk_placement_ptr, string_array[2].c_str());

  chunk_placement_ptr = our_double_buffer->get_chunk();
  strcpy(chunk_placement_ptr, string_array[3].c_str());
  our_double_buffer->start_processing();
  //This depends on how we return the result
  chunk_placement_ptr = our_double_buffer->get_result();

  //adding one chunk
  chunk_placement_ptr = our_double_buffer->get_chunk();
  strcpy(chunk_placement_ptr, string_array[4].c_str());
  our_double_buffer->start_processing();
  //This depends on how we return the result
  chunk_placement_ptr = our_double_buffer->get_result();



  return 0;
}
