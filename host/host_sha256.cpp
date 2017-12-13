#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>


bool sha256(char* input, unsigned long length, unsigned char* md) {
  SHA256_CTX context;
  if(!SHA256_Init(&context))
    return false;

  if(!SHA256_Update(&context, (unsigned char*)input, length))
    return false;

  if(!SHA256_Final(md, &context))
    return false;

  return true;
}

void file_sha256_cpu(const char *filename, int lines_to_read) {
  std::ifstream file;
  file.open(filename);

  // 64 is chunksize,
  char input[64];
  unsigned char output[SHA256_DIGEST_LENGTH];

  while (file >> input && lines_to_read > 0) {
    if (!sha256(input, strlen(input), output)) {
      printf("Error hashing with openssl\n");
    } else {
      for (int i = 0; i < 32; i++) {
	printf("%02x", output[i]);
      }
      printf("\n");
    }
    lines_to_read--;
  }
  file.close();
}

int main(int argc, char **argv) {
  file_sha256_cpu("passwords.txt", 18);
  return 0;
}
