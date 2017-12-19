#ifndef HOST_HOST_SHA256_HPP_
#define HOST_HOST_SHA256_HPP_

#include <vector>
#include <string>


std::vector<std::string> host_sha256_verify(std::string filename);
bool sha256(char* input, unsigned char* output);

#endif  // HOST_HOST_SHA256_HPP_
