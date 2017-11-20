#include <iostream>
#include <thread>
#include <string.h>
#include "double_buffer.hpp"
#include <vector>
#include <assert.h>
#include "defs.hpp"
class InterfaceTest
{
private:
  int chunks_per_buffer;
 
  Double_buffer *our_double_buffer; //= new Double_buffer(4);
  std::vector<std::string> string_array, result, expected_result;
  char *chunk_placement_ptr;
  void print_results();  
  void validate_result();
  void accuire_result();  
  void flipping_buffer(int amount_buffer_flips, int number_of_strings);
  void number_chunks();
 public:
  void run_tests();
  InterfaceTest();
  ~InterfaceTest();
};


InterfaceTest::InterfaceTest(){
  chunks_per_buffer = 7;
  our_double_buffer = new Double_buffer(chunks_per_buffer);
  
  string_array.push_back("111111111111111111111111111111111111111111111111111111111111111");
  string_array.push_back("222222222222222222222222222222222222222222222222222222222222222");
  string_array.push_back("333333333333333333333333333333333333333333333333333333333333333");
  string_array.push_back("444444444444444444444444444444444444444444444444444444444444444");
  string_array.push_back("555555555555555555555555555555555555555555555555555555555555555");



}
void InterfaceTest::print_results(){
  for (auto it = result.begin(); it != result.end(); ++it) {
    std::cout << *it << "\n";
  }
}
void InterfaceTest::validate_result(){
  for (int it = 0; it < result.size(); ++it) {
    
    std::string tmp1 = expected_result.back();
    std::string tmp2 = result.back();
    assert(tmp2.compare(tmp1) == 0);

  }
}
void InterfaceTest::accuire_result(){
  uint32_t offset = 64;
  std::string tmp;
  result_struct res = our_double_buffer->get_result();
  for (uint32_t i = 0; i < res.num_chunks; i++) {
    tmp = (res.res_ptr + i * offset);
    result.push_back(tmp);
  }
}

InterfaceTest::~InterfaceTest(){

}

void InterfaceTest::run_tests(){
  this->flipping_buffer(5,string_array.size());
  std::cout << "Validating flipping buffer " << "\n";
  this->number_chunks();
  std::cout << "Validating number_chunks in result_struct " << "\n";
}
void InterfaceTest::number_chunks(){

  chunk_placement_ptr = our_double_buffer->get_chunk();

  strncpy(chunk_placement_ptr, string_array[0].c_str(), 64);
  expected_result.push_back(string_array[0]);
  chunk_placement_ptr = our_double_buffer->get_chunk();
  strncpy(chunk_placement_ptr, string_array[1].c_str(), 64);
  expected_result.push_back(string_array[1]);
  
  our_double_buffer->start_processing();
  accuire_result();
  this->validate_result();
  
}

void InterfaceTest::flipping_buffer(int amount_buffer_flips, int number_of_strings){
  bool flag;
  int counter_flips = 0;
  int counter_chunks = 0;
  
  while (counter_flips < amount_buffer_flips) {
    flag = true;
    while (flag) {
      chunk_placement_ptr = our_double_buffer->get_chunk();
      if (chunk_placement_ptr == nullptr) {
        our_double_buffer->start_processing();
        accuire_result();
        flag = false;
      }else{
        strncpy(chunk_placement_ptr, string_array[counter_chunks % number_of_strings].c_str(), 64);
        expected_result.push_back(string_array[counter_chunks % number_of_strings]);
        counter_chunks++;

      }
    }
    counter_flips++;
  }
  this->validate_result();
}

int main(int argc, char *argv[]){
  InterfaceTest tester = InterfaceTest();
  tester.run_tests();  
  return 0;
}
