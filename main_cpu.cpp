#include <stdint.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>

#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include "device/double_buffer.hpp"
#include "device/defs.hpp"
#include "host/sha256.hpp"
#include "host/sha_preprocess.hpp"
#include "host/verify.hpp"


void print_result(struct buffer result) {
  for (int i = 0; i < result.num_chunks; i++) {
    for (int j = 0; j < 32; j++) {
      if (i >= CHUNKS_PER_BUFFER) {
        // forgott the offset here
        printf("%02x", ((unsigned char *)result.chunks[1][i - CHUNKS_PER_BUFFER].data)[j]);
      } else {
        printf("%02x", ((unsigned char *)result.chunks[0][i].data)[j]);
      }
    }
    printf("\n");
  }
}

void push_to_verify(struct buffer result, std::vector<std::string> *verify_vec) {
  char hashed_pass[65];
  for (int i = 0; i < result.num_chunks; i++) {
    int c = 0;
    for (int j = 0; j < 32; j++) {
       if (i >= CHUNKS_PER_BUFFER) {
         // forgott the offset here also
        c += snprintf(hashed_pass + c, 65-c, "%02x", ((unsigned char *) result.chunks[1][i - CHUNKS_PER_BUFFER].data)[j]);
      } else {
        c += snprintf(hashed_pass + c, 65-c, "%02x", ((unsigned char *) result.chunks[0][i].data)[j]);
      }
    }
    verify_vec->push_back(hashed_pass);
  }
}

void sha256_fpga(std::string filename, int lines_to_read, int dopt, int vopt) {
  DoubleBuffer *double_buffer;
  char *chunk_placement_ptr;
  int written_chunks = 0;
  struct buffer result;
  std::vector<std::string> verify_vec;

  double_buffer = new DoubleBuffer();
  std::fstream file;
  file.open(filename);

  while (1) {
    // if we want to keep going
    if (!file.eof() && lines_to_read != 0) {
      chunk_placement_ptr = double_buffer->get_chunk()->data;
    }
    // launch fpga
    if (chunk_placement_ptr == nullptr || file.eof() || lines_to_read == 0) {
      if (dopt) {
        std::cout << "get_chunk() returned nullptr" << std::endl;
        std::cout << "running start_processing().." << std::endl;
      }
      result = double_buffer->start_processing();
      written_chunks = 0;
      if (vopt) {
        push_to_verify(result, &verify_vec);
      }
      if (dopt) {
        print_result(result);
      }

      if (file.eof() || lines_to_read == 0) {
        break;
      }
    } else {
      file >> chunk_placement_ptr;

      // last read is eof and garbage
      // either process written chunks or exit
      if (file.eof()) {
        double_buffer->regret_get_chunk();
        if (written_chunks) {
          continue;
        } else {
          break;
        }
      }
      written_chunks++;
      if (dopt) {
        std::cout << "get_chunk() returned ptr" << std::endl;
        std::cout << "reading string from file: " << chunk_placement_ptr << std::endl;
      }
      pre_process(chunk_placement_ptr);
      lines_to_read--;
    }
  }

  result = double_buffer->get_last_result();
  if (dopt) {
    print_result(result);
  }
  if (vopt) {
    push_to_verify(result, &verify_vec);
    verify(verify_vec, filename);
  }

  file.close();
  delete double_buffer;
}

int main(int argc, char ** argv) {
  /*Initialization*/
  int c, filesize, lines_to_read = -1;
  int bopt = 0, dopt = 0, sopt = 0, fopt = 0, vopt = 0;
  double svalue;
  char *fvalue = NULL;
  std::string filename;
  std::chrono::duration<double> time_total;

  /*Getopt flags*/
  while ((c = getopt(argc,argv,"v,b,d,h,f:s:")) != -1) {
    switch (c) {
    case 'v': {
      vopt = 1;
      break;
    }
    case 'b': {
      bopt = 1;
      break;
    }
    case 'd': {
      dopt = 1;
      break;
    }
    case 'f': {
      fopt = 1;
      fvalue = optarg;
      break;
    }
    case 's': {
      sopt = 1;
      svalue = std::stod(optarg);
      break;
    }
    case 'h': {
      std::cout << "=============================== HELP PAGE ===================================" << std::endl;
      std::cout << "usage: ./main [-d] [-s size in MB] [-f filepath]" << std::endl;
      std::cout << "v : verification mode. Verifies results to a third-party program" << std::endl;
      std::cout << "d : debug mode. Displays print for the process of the program" << std::endl;
      std::cout << "s : define file size. Will read the whole file if not specified" << std::endl;
      std::cout << "f : define file to read. Will read password.txt if not specified" << std::endl;
      std::cout << "h : help page" << std::endl;
      std::cout << "==============================================================================" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    default: {
      std::cout << "Input was not recoqnized" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    }
  }

  std::cout << "======================== PRE SETTINGS ==========================" << std::endl;
  if (vopt) {
    std::cout << "verification mode is on" << std::endl;
  }

  if (dopt) {
    std::cout << "debug mode is on" << std::endl;
  }

  if (bopt) {
    std::cout << "benchmark mode is on" << std::endl;
  }

  if (fopt) { //filename flag
    filename = fvalue;
    std::cout << "filename: " << filename << std::endl;
  } else {
    const std::string& file_path = "cpu/random_passwords.txt";
    if (access(file_path.c_str(), F_OK ) != -1) {
      filename = "cpu/random_passwords.txt";
    } else {
      filename = "password.txt";
    }
    
    std::cout << "filename: " << filename << std::endl;
  }

  if (sopt) { //size flag
    filesize = svalue * 1000 * 1000;
    lines_to_read = trunc(filesize/64);
    std::cout << "size: " << svalue << "MB" << std::endl;
    std::cout << "lines_to_read: " << lines_to_read << std::endl;
  } else {
    std::cout << "size: whole file will be read" << std::endl;
  }

  std::cout << "================================================================" << std::endl;

  /*run sha256 fpga*/
  auto start = std::chrono::system_clock::now();
  sha256_fpga(filename, lines_to_read, dopt, vopt);
  auto end = std::chrono::system_clock::now();
  time_total = end - start;

  if (bopt) {
    std::cout << "Running sha256 CPU program..." << std::endl;
    auto start = std::chrono::system_clock::now();
    //benchmark program specified for hardware
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> cpu_program_time = end - start;
    std::cout << std::endl;
    std::cout << "====================== BENCHMARK RESULTS =======================" << std::endl;
    std::cout << "FPGA sha256 program time: "<< time_total.count() << "s" << std::endl;
    std::cout << std::endl;
    std::cout << "CPU sha256 program time: " <<  cpu_program_time.count() << "s" << std::endl;
    std::cout << "================================================================" << std::endl;
  }

  return 0;
}
