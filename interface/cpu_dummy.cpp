#include <iostream>
#include <thread>
#include <string.h>
#include <vector>

#include "double_buffer.hpp"
#include "defs.hpp"


class InterfaceTest {
private:
  DoubleBuffer *our_double_buffer;
  std::vector<std::string> string_array;
  char *chunk_placement_ptr;

  void test_switch_buffer_n_times(int amount_buffer_switches, int number_of_strings);
public:
  void run_tests();
  InterfaceTest();
  ~InterfaceTest();
};

InterfaceTest::InterfaceTest() {
  our_double_buffer = new DoubleBuffer();

  string_array.push_back("1111111111111111111111111111111111111111111111111111111111111111");
  string_array.push_back("2222222222222222222222222222222222222222222222222222222222222222");
  string_array.push_back("3333333333333333333333333333333333333333333333333333333333333333");
  string_array.push_back("4444444444444444444444444444444444444444444444444444444444444444");
  string_array.push_back("5555555555555555555555555555555555555555555555555555555555555555");
}

void InterfaceTest::run_tests() {
  this->test_switch_buffer_n_times(1, string_array.size());
}

void InterfaceTest::test_switch_buffer_n_times(int amount_buffer_switches, int number_of_strings) {
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
	for (int i = 0; i < result.num_chunks; i++) {
	  result.chunks[i].data[63] = '\0';
	  puts(result.chunks[i].data);
	}

        not_at_end_of_buffer = false;
      } else {
        strncpy(chunk_placement_ptr, string_array[counter_chunks % number_of_strings].c_str(), 64);
        counter_chunks++;
      }
    }
    counter_flips++;
  }
  result = our_double_buffer->get_last_result();
  for (int i = 0; i < result.num_chunks; i++) {
    result.chunks[i].data[63] = '\0';
    puts(result.chunks[i].data);
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
