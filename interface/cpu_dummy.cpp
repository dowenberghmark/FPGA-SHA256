#include <iostream>
#include <thread>
#include <string.h>
#include "double_buffer.hpp"


void deploy_chunk(std::string current, Double_buffer *double_buffer, char *chunk_placement_ptr){
  chunk_placement_ptr = double_buffer->get_chunk();
  if(chunk_placement_ptr != nullptr){

    std::cout << chunk_placement_ptr << "\n";
  }
}

void deploy_work(int total_chunks, int chunks_per_buffer, const std::string *data, Double_buffer *our_double_buffer){
  char *result;
  int counter = 0;
  char *chunk_placement_ptr;
  while (counter < total_chunks) {
    for (int i = 0; i < chunks_per_buffer ; i++) {
      std::cout << "The number: " << counter+i << "\n";
      deploy_chunk(data[i],our_double_buffer, chunk_placement_ptr);
      //counter++;
    }

    our_double_buffer->start_processing();
    std::cout << "start_proc\n" ;
    result = our_double_buffer->get_result();
    std::cout << "res\n" ;
    counter+= chunks_per_buffer;
  }
}

void flipping_buffer(int amount_buffer_flips, int number_of_strings, const std::string *data, Double_buffer *our_double_buffer){
  char *result;
  bool flag;
  int counter_flips = 0;
  int counter_chunks = 0;
  char *chunk_placement_ptr;

  while (counter_flips < amount_buffer_flips) {
    flag = true;
    while (flag) {
      chunk_placement_ptr = our_double_buffer->get_chunk();
      if (chunk_placement_ptr == nullptr) {
        our_double_buffer->start_processing();
        result = our_double_buffer->get_result();
        flag = false;
      }else{
        strncpy(chunk_placement_ptr, data[counter_chunks % number_of_strings].c_str(), 64);
      }
      counter_chunks++;
    }
    counter_flips++;
  }
}

int main(int argc, char *argv[])
{
  int number_of_strings = 5;
  int chunks_per_buffer = 2;
  Double_buffer *our_double_buffer = new Double_buffer(4);
  const std::string string_array[number_of_strings] = {"111111111111111111111111111111111111111111111111111111111111111",
                                    "222222222222222222222222222222222222222222222222222222222222222",
                                    "333333333333333333333333333333333333333333333333333333333333333",
                                    "444444444444444444444444444444444444444444444444444444444444444",
                                    "555555555555555555555555555555555555555555555555555555555555555"};

  //deploy_work(number_of_strings, chunks_per_buffer, string_array, our_double_buffer);
  flipping_buffer(50, number_of_strings, string_array, our_double_buffer);

  our_double_buffer->done();

  return 0;
}
