#include <iostream>
#include <thread>
#include <string.h>
#include "double_buffer.hpp"

void deploy_chunk(std::string current, Double_buffer *double_buffer){
  char *chunk_placement_ptr;
  chunk_placement_ptr = double_buffer->get_chunk();
  strcpy(chunk_placement_ptr, current.c_str());
}
void deploy_work(int total_chunks, int chunks_per_buffer, const std::string *data, Double_buffer *our_double_buffer){
  char *result;
  int counter = 0;
  while (counter < total_chunks) {
    for (int i = counter; i < chunks_per_buffer; i++) {
      deploy_chunk(data[1],our_double_buffer);
      counter++;
    }
     our_double_buffer->start_processing();
     result = our_double_buffer->get_result();
  }
}

int main(int argc, char *argv[])
{
  int number_of_strings = 5;
  int chunks_per_buffer = 2;
  Double_buffer *our_double_buffer = new Double_buffer(4);
  const std::string string_array[number_of_strings] = {"1111111111111111111111111111111111111111111111111111111111111111",
                                    "2222222222222222222222222222222222222222222222222222222222222222",
                                    "3333333333333333333333333333333333333333333333333333333333333333",
                                    "4444444444444444444444444444444444444444444444444444444444444444",
                                    "5555555555555555555555555555555555555555555555555555555555555555"};

  deploy_work(number_of_strings, chunks_per_buffer, string_array, our_double_buffer);

  return 0;
}
