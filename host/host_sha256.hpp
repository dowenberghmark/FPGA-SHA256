#ifndef __HOST_SHA256_H_INCLUDED__
#define __HOST_SHA256_H_INCLUDED__

#include <vector>


std::vector<std::string> host_sha256_verify(std::string filename);
bool sha256(char* input, unsigned char* output);

#endif
