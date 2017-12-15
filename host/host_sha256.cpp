#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdio>
#include <openssl/sha.h>
#include <vector>
#include <string>

#include "../device/defs.hpp"


bool sha256(char* input, unsigned char* output) {
  SHA256_CTX context;
  if(!SHA256_Init(&context))
    return false;

  if(!SHA256_Update(&context, (unsigned char*)input, strlen(input)))
    return false;

  if(!SHA256_Final(output, &context))
    return false;

  return true;
}

std::vector<std::string> host_sha256_verify(std::string filename){
  std::vector<std::string> hashes;
  std::ifstream file;
  char input[CHUNK_SIZE];
  unsigned char output[SHA256_DIGEST_LENGTH];

  file.open(filename);
  while(file >> input) {
    if (!sha256(input, output)) {
      printf("Error hashing with openssl\n");
    }
    std::string hash_str = "";
    char s[3];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
      sprintf(s, "%02x", output[i]);
      hash_str += s;
    }
    hashes.push_back(hash_str);
  }

  file.close();
  return hashes;
}
