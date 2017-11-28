#include<stdio.h>
#include<stdint.h>
#include "sha256.h"
#include "sha_preprocess.hpp"

main(){
    char* test = "hej";
    printf("%s\n", "1");
    pre_process(test);
    printf("%s\n", "1");
   	sha256((uint32_t*)test);

    printf((char*)test);

}