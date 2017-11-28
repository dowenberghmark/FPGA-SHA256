#include <iostream>
#include <thread>
#include <string.h>
#include <vector>

#include "double_buffer.hpp"
#include "defs.hpp"


class InterfaceTest {
private:
  DoubleBuffer *our_double_buffer; //= new DoubleBuffer(4);
  std::vector<std::string> string_array;
  char *chunk_placement_ptr;

  void test_switch_buffer_n_times(int amount_buffer_switches, int number_of_strings);
public:
  void run_tests();
  InterfaceTest();
  ~InterfaceTest() = default;
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
  this->test_switch_buffer_n_times(10,string_array.size());
}

void InterfaceTest::test_switch_buffer_n_times(int amount_buffer_switches, int number_of_strings) {
  struct chunk *result;
  bool not_at_end_of_buffer;
  int counter_flips = 0;
  int counter_chunks = 0;
  char *chunk_placement_ptr;
  while (counter_flips < amount_buffer_switches) {
    not_at_end_of_buffer = true;
    while (not_at_end_of_buffer) {
      chunk_placement_ptr = (char *) our_double_buffer->get_chunk();
      if (chunk_placement_ptr == nullptr) {
        result = our_double_buffer->start_processing();
        not_at_end_of_buffer = false;
      } else {
        strncpy(chunk_placement_ptr, string_array[counter_chunks % number_of_strings].c_str(), 64);
        counter_chunks++;
      }
    }
    counter_flips++;
  }
}

int main(int argc, char *argv[]) {
  InterfaceTest tester = InterfaceTest();
  tester.run_tests();
  return 0;
}
