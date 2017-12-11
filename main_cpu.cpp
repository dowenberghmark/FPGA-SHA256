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

#include "interface/double_buffer.hpp"
#include "interface/defs.hpp"
#include "cpu/sha256.hpp"
#include "cpu/sha_preprocess.hpp"
#include "cpu/verify.hpp"


void sha256_fpga(std::string filename,int lines_to_read,int dopt) {
  DoubleBuffer *double_buffer;
  char *chunk_placement_ptr;

  double_buffer = new DoubleBuffer();
  std::fstream file;
  file.open(filename);

  while (!file.eof()) {

    chunk_placement_ptr = double_buffer->get_chunk()->data;

    if (dopt == 1) {
      std::cout << "reading string: " << element << std::endl;
    }

    if (chunk_placement_ptr == nullptr) {
      if (dopt == 1) {
        std::cout << "get_chunk() returned nullptr" << std::endl;
        std::cout << "running start_processing().." << std::endl;
      }
      result = double_buffer->start_processing();
      if (dopt == 1) {
        for (int i=0;i<result.num_chunks;i++) {
	  for (int j = 0; j < 32; j++) {
	    printf("%02x", ((unsigned char *)result.chunks[i].data)[j]);
	  }
	  printf("\n");
        }
      }

      chunk_placement_ptr = double_buffer->get_chunk()->data;
    }
      if (dopt == 1) {
  	std::cout << "get_chunk() returned value " << std::endl;
      }

    memset(chunk_placement_ptr,0,64);
    file >> chunk_placement_ptr;
    /* should always run this part */
    pre_process(chunk_placement_ptr);
    lines_to_read--;

    if (lines_to_read == 0) {
      break;
    }
  }
  result = double_buffer->start_processing();
  result = double_buffer->get_last_result();

  if (dopt == 1) {
    for (int i=0;i<result.num_chunks;i++) {
      for (int j = 0; j < 32; j++) {
        printf("%02x", ((unsigned char *)result.chunks[i].data)[j]);
      }
      printf("\n");
    }
  }

  file.close();
  delete double_buffer;
}

int main(int argc, char ** argv) {
  /*Initialization*/
  int c, svalue, filesize, lines_to_read = -1;
  int bopt = 0, dopt = 0, sopt = 0, fopt = 0, vopt = 0;
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
      svalue = std::stoi(optarg);
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
  if (vopt == 1) {
    std::cout << "verification mode is on" << std::endl;
  }

  if (dopt == 1) {
    std::cout << "debug mode is on" << std::endl;
  }

  if (bopt == 1) {
    std::cout << "benchmark mode is on" << std::endl;
  }

  if (fopt == 1) { //filename flag
    filename = fvalue;
    std::cout << "filename: " << filename << std::endl;
  } else {
    filename = "password.txt";
    std::cout << "filename: " << filename << std::endl;
  }

  if (sopt == 1) { //size flag
    filesize = svalue * 1000;
    lines_to_read = trunc(filesize/64);
    std::cout << "size: " << filesize << "MB" << std::endl;
    std::cout << "lines_to_read: " << lines_to_read << std::endl;
  } else {
    std::cout << "size: whole file will be read" << std::endl;
  }

  std::cout << "================================================================" << std::endl;

  /*run sha256 fpga*/
  auto start = std::chrono::system_clock::now();
  sha256_fpga(filename,lines_to_read,dopt);
  auto end = std::chrono::system_clock::now();
  time_total = end - start;

  if (bopt == 1) {
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

  if (vopt == 1) {
    std::cout << "====================== VERIFICATION RESULTS =======================" << std::endl;
    verify(filename);
    std::cout << "================================================================" << std::endl;
  }
  return 0;
}
