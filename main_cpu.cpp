#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>

#include "interface/double_buffer.hpp"
#include "interface/defs.hpp"
#include "cpu/sha256.hpp"
#include "cpu/sha_preprocess.hpp"

int main(int argc, char ** argv) {
  /*Initialization*/
  int c;
  int bopt = 0, dopt = 0, sopt = 0;
  int fopt = 0;
  int svalue;
  int filesize;
  int lines_to_read;
  char *fvalue = NULL;
  char const *dram_path;
  char element[64];
  std::string filename;
  DoubleBuffer *our_double_buffer;
  char *chunk_placement_ptr;
  std::chrono::duration<double> time_preprocess;
  std::chrono::duration<double> time_buffer_processing;
  std::chrono::duration<double> time_retrive_result;
  std::chrono::duration<double> time_total;
  auto start_preprocess = std::chrono::system_clock::now();
  auto end_preprocess = std::chrono::system_clock::now();
  auto start_buffer_processing = std::chrono::system_clock::now();
  auto end_buffer_processing = std::chrono::system_clock::now();
  auto start_retrive_result = std::chrono::system_clock::now();
  auto end_retrive_result = std::chrono::system_clock::now();

  /*Getopt flags*/
  while ((c = getopt (argc,argv,"b,d,h,f:s:")) != -1) {
    switch (c) {
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
        std::cout << "d : debug mode. Displays print for the process of the program" << std::endl;
        std::cout << "s : define file size. Will read the whole file if not specified" << std::endl;
        std::cout << "f : define file to read. Will read password.txt if not specified" << std::endl;
        std::cout << "h : help page" << std::endl;
        std::cout << "==============================================================================" << std::endl;
        std::exit(EXIT_SUCCESS);
      }
      default: {
        std::cout << "Input was not recoqnized" << std::endl;
        std::exit(EXIT_SUCCESS);
      }
    }
  }

  std::cout << "======================== PRE SETTINGS ==========================" << std::endl;

  if (dopt == 1) {
    std::cout << "debug mode is on" << std::endl;
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

  if (MODE == LOCAL) {
    dram_path = "dram.hex";
    std::cout << "LOCAL MODE" << std::endl;
  } else if (MODE == AWS) {
    dram_path = "/dev/edma0_queue_0";
    std::cout << "AWS MODE" << std::endl;
  }

  std::cout << "reading from dram: " << dram_path << std::endl;
  std::cout << "================================================================" << std::endl;

  /*Initialization*/
  std::fstream file;
  file.open(filename);
  our_double_buffer = new DoubleBuffer(dram_path);
  auto start = std::chrono::system_clock::now();  //benchmark start

  /*start reading file rows*/
  while (!file.eof()) {
    memset(element,0,64);
    file >> element;
    chunk_placement_ptr = our_double_buffer->get_chunk();

    if (dopt == 1) {
      std::cout << "reading string: " << element << std::endl;
    }

    if (chunk_placement_ptr == nullptr) {
      if (dopt == 1) {
	std::cout << "get_chunk() returned nullptr" << std::endl;
	std::cout << "running start_processing() & get_result().." << std::endl;
      }

      start_buffer_processing = std::chrono::system_clock::now();
      our_double_buffer->start_processing(); 
      end_buffer_processing = std::chrono::system_clock::now();
      time_buffer_processing += end_buffer_processing - start_buffer_processing;
      start_retrive_result = std::chrono::system_clock::now();
      our_double_buffer->get_result();
      end_retrive_result = std::chrono::system_clock::now();
      time_retrive_result += end_retrive_result - start_retrive_result;

      if (dopt == 1) {
	std::cout << "get_chunk() returned: " << &chunk_placement_ptr << std::endl;
      }

      chunk_placement_ptr = our_double_buffer->get_chunk();
    } 
    start_preprocess = std::chrono::system_clock::now();
    /* should always run this part */
    pre_process(element);
    memcpy(chunk_placement_ptr,element,sizeof(element));
    end_preprocess = std::chrono::system_clock::now();
    time_preprocess += end_preprocess - start_preprocess;
    lines_to_read--;

    if (lines_to_read == 0) {
      break;
    }
  }
  our_double_buffer->done();
  auto end = std::chrono::system_clock::now();
  file.close();
  time_total = end - start;

  if (bopt == 1) { 
    std::cout << "Running sha256 CPU program..." << std::endl;
    auto start = std::chrono::system_clock::now();
    SHA256_CPU_benchmark(filename);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> cpu_program_time = end - start;
    std::cout << std::endl;
    std::cout << "====================== BENCHMARK RESULTS =======================" << std::endl;
    std::cout << "FPGA sha256 program time: "<< time_total.count() << "s" << std::endl;
    std::cout << std::endl;
    std::cout << "CPU sha256 program time: " <<  cpu_program_time.count() << "s" << std::endl;
    std::cout << "================================================================" << std::endl;
  }
  if (dopt == 1) {
    std::cout << "====================== DEBUG TIME RESULTS =======================" << std::endl;
    std::cout << "pre_process() time: " << time_preprocess.count() << "s" << std::endl;
    std::cout << "start_processing() function time: " << time_buffer_processing.count() << "s" << std::endl;
    std::cout << "get_result() function time: " << time_retrive_result.count() << "s" << std::endl;
    std::cout << "total program time: "<< time_total.count() << "s" << std::endl;
    std::cout << "================================================================" << std::endl;
  }
  return 0;
}
