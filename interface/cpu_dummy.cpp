#include <iostream>
#include <thread>
#include <string.h>
#include "double_buffer.hpp"
#include <vector>
class InterfaceTest
{
private:
  int chunks_per_buffer;
 
  Double_buffer *our_double_buffer; //= new Double_buffer(4);
  std::vector<std::string> string_array;
  char *chunk_placement_ptr;

  
  void flipping_buffer(int amount_buffer_flips, int number_of_strings);
  void deploy_work(int total_chunks, int chunks_per_buffer);
  void deploy_chunk(std::string current);  
public:
  void run_tests();
  InterfaceTest();
  ~InterfaceTest();
};


InterfaceTest::InterfaceTest(){
  chunks_per_buffer = 4;
  our_double_buffer = new Double_buffer(chunks_per_buffer);
  
  string_array.push_back("111111111111111111111111111111111111111111111111111111111111111");
  string_array.push_back("222222222222222222222222222222222222222222222222222222222222222");
  string_array.push_back("333333333333333333333333333333333333333333333333333333333333333");
  string_array.push_back("444444444444444444444444444444444444444444444444444444444444444");
  string_array.push_back("555555555555555555555555555555555555555555555555555555555555555");



}

InterfaceTest::~InterfaceTest(){

}

void InterfaceTest::run_tests(){
  this->flipping_buffer(40,string_array.size());
}


void InterfaceTest::deploy_chunk(std::string current){
  chunk_placement_ptr = our_double_buffer->get_chunk();
  if(chunk_placement_ptr != nullptr){

    std::cout << chunk_placement_ptr << "\n";
  }
}

void InterfaceTest::deploy_work(int total_chunks, int chunks_per_buffer){
  char *result;
  int counter = 0;
  while (counter < total_chunks) {
    for (int i = 0; i < chunks_per_buffer ; i++) {
      std::cout << "The number: " << counter+i << "\n";
      deploy_chunk(string_array[i]);
      //counter++;
      
    }
    our_double_buffer->start_processing();
    std::cout << "start_proc\n" ;
    result = our_double_buffer->get_result();
    std::cout << "res\n" ;
    counter+= chunks_per_buffer;
  }
}

void InterfaceTest::flipping_buffer(int amount_buffer_flips, int number_of_strings){
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
        strncpy(chunk_placement_ptr, string_array[counter_chunks % number_of_strings].c_str(), 64);
      }
      counter_chunks++;
    }
    counter_flips++;
  }
}


int main(int argc, char *argv[])
{

  InterfaceTest tester = InterfaceTest();
  tester.run_tests();  
  return 0;
}
