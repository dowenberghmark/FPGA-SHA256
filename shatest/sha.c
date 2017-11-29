#include <stdio.h>
#include <stdint.h>
#include "sha256.h"
#include "sha_preprocess.hpp"

main(){
  char test[64] = {'\0'};
  test[0]= 'a'; //hash of "a" is
  //ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb
  //14ffffffd660ffffffa16115ffffffa1ffffffd1
  //printf("%s\n", "1");
  pre_process(test);
  //printf("%s\n", "1");
  sha256((uint32_t*)test);

  for (int i = 0; i < 8; i+=sizeof(uint32_t)){
    printf("%x\n", (uint32_t)test[i]);
  }
  
}
