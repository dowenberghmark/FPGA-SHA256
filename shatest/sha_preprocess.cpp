#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <string.h>
#include <stdint.h>

using namespace std;

#define uchar unsigned char
#define uint unsigned int

void pre_process(char *element){
  /*Initilaize*/
  int num = 1;
  int strLen = strlen(element);
  uint64_t l_append;
  uchar one;
  /*appends 1 after string*/

    if(*(char *)&num == 1) {   // Little-Endian
      one = 0x80;
    } else {  // Big-Endian
      one = 1;
    }

    element[strLen++] = one;
    strLen--;
    l_append = strLen*8;

    if(*(char *)&num == 1) {  // if Little-Endian
      l_append = strLen*8;
      element[56] = (l_append >> 56) & 0xFF;
      element[57] = (l_append >> 48) & 0xFF;
      element[58] = (l_append >> 40) & 0xFF;
      element[59] = (l_append >> 32) & 0xFF;
      element[60] = (l_append >> 24) & 0xFF;
      element[61] = (l_append >> 16) & 0xFF;
      element[62] = (l_append >> 8) & 0xFF;
      element[63] = l_append & 0xFF;
    } else{} // Big-Endian
  }