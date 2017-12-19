#include <string.h>
#include <stdint.h>

#include "sha_preprocess.hpp"

void pre_process(char *element) {
  /*Initilaize*/
  int str_len = strlen(element);
  uint64_t element_length_append;
  element_length_append = str_len*8;
  element[str_len] = 0x80;
  memset(element+str_len+1, 0, 55-str_len);

  element[56] = (element_length_append >> 56) & 0xFF;
  element[57] = (element_length_append >> 48) & 0xFF;
  element[58] = (element_length_append >> 40) & 0xFF;
  element[59] = (element_length_append >> 32) & 0xFF;
  element[60] = (element_length_append >> 24) & 0xFF;
  element[61] = (element_length_append >> 16) & 0xFF;
  element[62] = (element_length_append >> 8) & 0xFF;
  element[63] = element_length_append & 0xFF;
}
