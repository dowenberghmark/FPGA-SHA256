#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <chrono>
#include <bitset>
#include <iomanip>
#include <string.h>

#include "interface/double_buffer.hpp"
#include "interface/defs.hpp"


using namespace std;

#define uchar unsigned char
#define uint unsigned int


void pre_process(char* element){
/*appends 1 after string*/
  int num = 1;
  unsigned char one;
    if(*(char *)&num == 1){   // Little-Endian
      one = 1 << 7;
    } else{  // Big-Endian
      one = 1;
    }
    int strLen = strlen(element);
    element[strLen++] = one;
    strLen--;
    unsigned char bytes[4];
    char l_append;
    l_append = strLen*8;

    if(*(char *)&num == 1){  // if Little-Endian
      l_append = strLen*8;
      bytes[0] = (l_append >> 24) & 0xFF;
      bytes[1] = (l_append >> 16) & 0xFF;
      bytes[2] = (l_append >> 8) & 0xFF;
      bytes[3] = l_append & 0xFF;
      element[60] = bytes[0];
      element[61] = bytes[1];
      element[62] = bytes[2];
      element[63] = bytes[3];    
    } else{} // Big-Endian
  }

  int main() {
    char* string_array;
    char *chunk_placement_ptr;
    char const *dram_path;
    char element[64];

    fstream file;
    file.open("password.txt");

    DoubleBuffer *our_double_buffer;

    if (MODE == LOCAL) {
      dram_path = "dram.hex";
    } else if (MODE == AWS) {
      dram_path = "/dev/edma0_queue_0";
    }  

    our_double_buffer = new DoubleBuffer(dram_path);

    while(!file.eof()){
      memset(element,0,64);
      file >> element;
      chunk_placement_ptr = our_double_buffer->get_chunk();

      if(chunk_placement_ptr == nullptr){
        our_double_buffer->start_processing();
        our_double_buffer->get_result();
      } else{
       pre_process(element);      
       memcpy(chunk_placement_ptr,element,sizeof(element));
     }

   }

   our_double_buffer->done();
   return 0;
 }