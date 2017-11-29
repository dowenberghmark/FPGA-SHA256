#include <stdio.h>
#include <stdint.h>
#include "sha256.h"
#include "sha_preprocess.hpp"

int char_to_uint32_array(char *chars, int char_length, uint32_t* new_array){
	if(!(char_length > 0) | chars == NULL) {
		return 1;
	}else{
		int j = 0;
		for(int i = 0; i < char_length; i+=4){
			uint32_t a = ((0xffff & chars[i])<<8 | (0xffff & chars[i+1]));
			printf("part1: %x\n", a);
			uint32_t b = ((0xffff & chars[i+2])<<8 | (0xffff & chars[i+3]));
			printf("part1: %x\n", b);
			new_array[j] = ((0xffffffff & a)<<16 | (0xffffffff & b));
			printf("all: %x\n", new_array[j]);
			j=j+1;
			//new_array[i] = (((0xffffffff & chars[i]) << 3*8) | ((0xffffffff & chars[i+1]) << 2*8) | ((0xffffffff & chars[i+2]) << 1*8) | (0xffffffff & chars[i+3]));
		}
		return 0;
	}
}

main(){
	char test[64] = {'\0'};
	test[0]= 'a'; //hash of "a" is
	//ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb
	//14ffffffd660ffffffa16115ffffffa1ffffffd1
	//printf("%s\n", "1");
	//pre_process(test);
	//printf("%s\n", "1");
	//  sha256((uint32_t*)test);

	char *test2 = "abcdefghijklmnoq";
	int strlenw = 16;
	//uint32_t *test3;
	uint32_t test3[strlenw/4];
	char_to_uint32_array(test2, strlenw, test3);
	printf("%s\n", test2);
	for (int i = 0; i < strlenw/4; ++i){
		printf("Tet: %x\n", test3[i]);
	}

	//for (int i = 0; i < 8; i+=sizeof(uint32_t)){
		//    printf("%x\n", (uint32_t)test[i]);
	//}
}
