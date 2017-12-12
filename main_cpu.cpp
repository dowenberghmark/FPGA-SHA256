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

typedef struct pre_settings_t {
  double svalue;
  int filesize, lines_to_read;
  int bopt, dopt, sopt, fopt, vopt;
  char *fvalue;
  std::string filename;
}settings;

void pre_settings_init(settings *config) {
  config->svalue = -1;
  config->filesize = -1;
  config->lines_to_read = -1;
  config->bopt = 0;
  config->dopt = 0;
  config->sopt = 0;
  config->fopt = 0;
  config->vopt = 0;
  config->fvalue = NULL;
  config->filename = NULL;
}



void pre_settings(settings *config){
  std::cout << "======================== PRE SETTINGS ==========================" << std::endl;
  if (config->vopt == 1) {
    std::cout << "verification mode is on" << std::endl;
  }

  if (config->dopt == 1) {
    std::cout << "debug mode is on" << std::endl;
  }

  if (config->bopt == 1) {
    std::cout << "benchmark mode is on" << std::endl;
  }

  if (config->fopt == 1) { //filename flag
    config->filename = config->fvalue;
    std::cout << "filename: " << config->filename << std::endl;
  } else {
    config->filename = "password.txt";
    std::cout << "filename: " << config->filename << std::endl;
  }

  if (config->sopt == 1) { //size flag
    config->filesize = config->svalue * 1000 * 1000; //2pow20
    config->lines_to_read = trunc(config->filesize/64);
    std::cout << "size: " << config->filesize << "MB" << std::endl;
    std::cout << "lines_to_read: " << config->lines_to_read << std::endl;
  } else {
    std::cout << "size: whole file will be read" << std::endl;
  }
  std::cout << "================================================================" << std::endl;
}

void benchmark(int time_diff){
  std::cout << "Running sha256 CPU program..." << std::endl;
  auto start = std::chrono::system_clock::now();
  //benchmark program specified for hardware
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> cpu_program_time = end - start;
  std::cout << std::endl;
  std::cout << "====================== BENCHMARK RESULTS =======================" << std::endl;
  std::cout << "FPGA sha256 program time: "<< time_diff << "s" << std::endl;
  std::cout << std::endl;
  std::cout << "CPU sha256 program time: " <<  cpu_program_time.count() << "s" << std::endl;
  std::cout << "================================================================" << std::endl;
}

void help(){
  std::cout << "=============================== HELP PAGE ===================================" << std::endl;
  std::cout << "usage: ./main [-d] [-s size in MB] [-f filepath]" << std::endl;
  std::cout << "v : verification mode. Verifies results to a third-party program" << std::endl;
  std::cout << "d : debug mode. Displays print for the process of the program" << std::endl;
  std::cout << "s : define file size. Will read the whole file if not specified" << std::endl;
  std::cout << "f : define file to read. Will read password.txt if not specified" << std::endl;
  std::cout << "h : help page" << std::endl;
  std::cout << "==============================================================================" << std::endl;
}

void print_result(struct buffer result) {
  for (int i = 0; i < result.num_chunks; i++) {
    for (int j = 0; j < 32; j++) {
      printf("%02x", ((unsigned char *)result.chunks[i].data)[j]);
    }
    printf("\n");
  }
}

void sha256_fpga(settings *config) {

  DoubleBuffer *double_buffer;
  char *chunk_placement_ptr;
  int written_chunks = 0;
  struct buffer result;

  double_buffer = new DoubleBuffer();
  std::fstream file;
  file.open(config->filename);

  while (1) {
    // if we want to keep going
    if (!file.eof() && config->lines_to_read != 0) {
      chunk_placement_ptr = double_buffer->get_chunk()->data;
    }
    // launch fpga
    if (chunk_placement_ptr == nullptr || file.eof() || config->lines_to_read == 0) {
      if (config->dopt) {
        std::cout << "get_chunk() returned nullptr" << std::endl;
        std::cout << "running start_processing().." << std::endl;
      }
      result = double_buffer->start_processing();
      written_chunks = 0;
      if (config->dopt) {
        print_result(result);
      }

      if (file.eof() || config->lines_to_read == 0) {
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
      if (config->dopt) {
	std::cout << "get_chunk() returned ptr" << std::endl;
	std::cout << "reading string from file: " << chunk_placement_ptr << std::endl;
      }
      pre_process(chunk_placement_ptr);
      config->lines_to_read--;
    }
  }

  result = double_buffer->get_last_result();

  if (config->dopt) {
    print_result(result);
  }

  file.close();
  delete double_buffer;
}

int main(int argc, char ** argv) {
  /*Initialization*/

  std::chrono::duration<double> time_total;
  settings pre_sets;
  pre_settings_init(&pre_sets);

  /*Getopt flags*/
  int c;
  while ((c = getopt(argc,argv,"v,b,d,h,f:s:")) != -1) {
    switch (c) {

    case 'v': {
      pre_sets.vopt = 1;
      break;
    }
    case 'b': {
      pre_sets.bopt = 1;
      break;
    }
    case 'd': {
      pre_sets.dopt = 1;
      break;
    }
    case 'f': {
      pre_sets.fopt = 1;
      pre_sets.fvalue = optarg;
      break;
    }
    case 's': {
      pre_sets.sopt = 1;
      pre_sets.svalue = std::stod(optarg);
      break;
    }
    case 'h': {
      help();
      std::exit(EXIT_FAILURE);
    }
    default: {
      std::cout << "Input was not recoqnized" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    }
  }

  pre_settings(&pre_sets);


  if (pre_sets.vopt == 1) {
    std::cout << "====================== VERIFICATION RESULTS =======================" << std::endl;
    std::cout << "================================================================" << std::endl;
  }
  else {
    auto start = std::chrono::system_clock::now();
    sha256_fpga(&pre_sets);
    auto end = std::chrono::system_clock::now();
    if (pre_sets.bopt == 1) {
      time_total = end - start;
      benchmark(time_total.count());
    }
  }

  return 0;
}
