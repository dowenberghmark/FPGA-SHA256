#include <stdio.h>
#include <stdint.h>
#include "sha256.h"
#include "sha_preprocess.hpp"


uint32_t *char_to_uint32_array(char *chars, int char_length){
	if(!(char_length > 0) | chars == NULL) {
		return NULL;
	}else{
		uint32_t new_array[char_length/4];
		for(int i = 0; i < char_length; i+=4) {
			new_array[i] = (((0xffffffff & chars[i]) << 3) &\
				((0xffffffff & chars[i+1]) << 2) &\
				((0xffffffff & chars[i+2]) << 1) &\
				(0xffffffff & chars[i+3]));
		}
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

	char *test2 = "abcd";
	uint32_t *test3;
	test3 = char_to_uint32_array(test2, 4);
	printf("Test: %x\n", test3);

	//for (int i = 0; i < 8; i+=sizeof(uint32_t)){
	//    printf("%x\n", (uint32_t)test[i]);
	//}

}
