#include <iostream>
#include <fstream>
#include <cstdio>
#include <string.h>
#include <openssl/sha.h>
#include <chrono>
#include <ctime>
#include <climits>
#include <cstdlib>
#include <unistd.h>
#include <cmath>

#define MEGABYTE 1000000


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

void file_sha256(std::ifstream& file, int lines_to_read) {
  // 64 is chunksize,
  char input[64];
  unsigned char output[SHA256_DIGEST_LENGTH];

  while (file >> input && lines_to_read > 0) {
    if (!sha256(input, strlen(input), output)) {
      printf("Error hashing with openssl\n");
    }
    lines_to_read--;
  }
}

void benchmark_host_sha256(const char *filename, int lines_to_read) {
  std::ifstream file;
  file.open(filename);
  auto start = std::chrono::system_clock::now();
  file_sha256(file, lines_to_read);
  auto end = std::chrono::system_clock::now();
  file.close();
  std::chrono::duration<double> diff = end - start;

  std::cout << "====================== BENCHMARK RESULTS =======================" << std::endl;
  std::cout << "Host sha256 program time: " <<  diff.count() << "s" << std::endl;
  std::cout << "================================================================" << std::endl;
}

int main(int argc, char **argv) {
  double svalue;
  char *filename = (char *) "passwords.txt";
  int c, lines_to_read = INT_MAX;

  /* Getopt flags */
  while ((c = getopt(argc,argv,"h,f:s:")) != -1) {
    switch (c) {
    case 'f': {
      filename = optarg;
      break;
    }
    case 's': {
      svalue = std::stod(optarg);
      lines_to_read = trunc((svalue * MEGABYTE) / 64);
      break;
    }
    case 'h': {
      std::cout << "=============================== HELP PAGE ===================================" << std::endl;
      std::cout << "usage: ./host_sha256 [-s size in MB] [-f filepath]" << std::endl;
      std::cout << "s : define file size. Will read the whole file if not specified" << std::endl;
      std::cout << "f : define file to read. Will read password.txt if not specified" << std::endl;
      std::cout << "h : help page" << std::endl;
      std::cout << "==============================================================================" << std::endl;
      std::exit(EXIT_SUCCESS);
    }
    default: {
      std::cout << "Input was not recoqnized" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    }
  }

  benchmark_host_sha256(filename, lines_to_read);
  return 0;
}
