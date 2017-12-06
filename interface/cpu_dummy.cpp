#include <iostream>
#include <string.h>

#include "double_buffer.hpp"
#include "defs.hpp"
#include "../cpu/sha_preprocess.hpp"

class InterfaceTest {
private:
  DoubleBuffer *our_double_buffer;
  chunk test_chunks[10] = {"testing", "adam", "sara", "emil", "oliver", "daniel", "carl", "andy", "jeff", "viktor"};
  char *chunk_placement_ptr;
  void test_switch_buffer_n_times(int amount_buffer_switches, int number_of_strings);
public:
  void run_tests();
  InterfaceTest();
  ~InterfaceTest();
};

InterfaceTest::InterfaceTest() {
  our_double_buffer = new DoubleBuffer();
}

void InterfaceTest::run_tests() {
  this->test_switch_buffer_n_times(3, 10);
}

void InterfaceTest::test_switch_buffer_n_times(int amount_buffer_switches, int number_of_strings) {
  //preprocess data chunks
  for(int i = 0; i < 10; i++) {
    pre_process(test_chunks[i].data);
  }

  struct buffer result;
  bool not_at_end_of_buffer;
  int counter_flips = 0;
  int counter_chunks = 0;
  char *chunk_placement_ptr;
  
  while (counter_flips < amount_buffer_switches) {
    not_at_end_of_buffer = true;
    while (not_at_end_of_buffer) {
      chunk_placement_ptr = (char *) our_double_buffer->get_chunk()->data;    
      if (chunk_placement_ptr == nullptr) {
	result = our_double_buffer->start_processing();
	//print resulting hash
        for (int i = 0; i < result.num_chunks; i++) {
	  puts("Hash output:");
	  for(int j = 0; j < 32; j++) {
	    printf("%02x", ((unsigned char *)result.chunks[i].data)[j]);
	  }
	  puts("");
        }	
        not_at_end_of_buffer = false;
      } else {
	//put data chunks from test chunks into chunk buffer, wraparound when out of test chunks
        memcpy(chunk_placement_ptr, test_chunks[counter_chunks % number_of_strings].data, 64);
        counter_chunks++;
      }
    }
    counter_flips++;
  }
  
  result = our_double_buffer->start_processing();
  //print resulting hash
  for (int i = 0; i < result.num_chunks; i++) {
    puts("Hash output:");
    for(int j = 0; j < 32; j++) {
      printf("%02x", ((unsigned char *)result.chunks[i].data)[j]);
    }
    puts("");
  }
}

InterfaceTest::~InterfaceTest() {
  delete our_double_buffer;
}

int main(int argc, char *argv[]) {
  InterfaceTest *tester = new InterfaceTest();
  tester->run_tests();
  delete tester;
  return 0;
}
