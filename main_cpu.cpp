#include <unistd.h>
#include <stdint.h>
#include <tgmath.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#include "device/double_buffer.hpp"
#include "device/defs.hpp"
#include "host/sha_preprocess.hpp"
#include "host/verify.hpp"

#define MB (1000 * 1000)

// global variables referenced in defs.hpp
size_t BUFFER_SIZE;
int CHUNKS_PER_BUFFER;

typedef struct pre_settings_t {
  double amount_to_process, buffer_size;
  int lines_to_read;
  int bopt, dopt, sopt, fopt, vopt, oopt;
  char *fvalue;
  std::string filename, outfile;
}settings;

void pre_settings_init(settings *config) {
  config->amount_to_process = -1;
  config->lines_to_read = -1;
  config->bopt = 0;
  config->dopt = 0;
  config->sopt = 0;
  config->fopt = 0;
  config->vopt = 0;
  config->oopt = 0;
  config->fvalue = NULL;
  config->filename = "./password.txt";
  config->outfile = "./results/output.csv";
  config->buffer_size = 1000;  // 4 chunks per buffer
}

void set_lines_to_read(settings *config) {
  config->lines_to_read = trunc(config->amount_to_process * MB/CHUNK_SIZE);
  std::cout << "processing size: " << config->amount_to_process << " MB" << std::endl;
  std::cout << "lines_to_read: " << config->lines_to_read << std::endl;
}

void pre_settings(settings *config) {
  std::cout << "======================== PRE SETTINGS ==========================" << std::endl;
  if (config->vopt) {
    std::cout << "verification mode is on" << std::endl;
  }
  if (config->dopt) {
    std::cout << "debug mode is on" << std::endl;
  }
  if (config->bopt) {
    std::cout << "benchmark mode is on" << std::endl;
    std::cout << "output file is set to: " << config->outfile << std::endl;
  }
  if (config->fopt) {  // filename flag
    if (access(config->fvalue, F_OK) != -1) {
      config->filename = config->fvalue;
    } else {
      std::cout << "File does not exist" << std::endl;
      std::exit(EXIT_FAILURE);
    }
    std::cout << "filename: " << config->filename << std::endl;
  } else {
    const std::string& file_path = "host/random_passwords.txt";
    if (access(file_path.c_str(), F_OK) != -1) {
      config->filename = "host/random_passwords.txt";
    } else {
      config->filename = "password.txt";
    }
    std::cout << "filename: " << config->filename << std::endl;
  }
  if (config->sopt) {
    set_lines_to_read(config);
  } else {
    std::cout << "processing size: whole file will be read" << std::endl;
  }
  // Bytes to MB
  BUFFER_SIZE = ceil(config->buffer_size*MB);
  CHUNKS_PER_BUFFER = ceil(BUFFER_SIZE / CHUNK_SIZE);
  std::cout << "buffer size: " << config->buffer_size << " MB, " << CHUNKS_PER_BUFFER << " chunks per buffer" << std::endl;

  std::cout << "================================================================" << std::endl;
}

void help() {
  std::cout << "================================ HELP PAGE ===================================" << std::endl;
  std::cout << "usage: ./main [-b] [-o outputfile] [-d] [-s size in MB] [-f filepath] [-B buffersize in MB]" << std::endl;
  std::cout << "b : benchmark mode. Will append run time info to results/output.csv if -o flag is not used" << std::endl;
  std::cout << "o : specify output file for benchmarking. the output file is set to results/output.csv as default" << std::endl;
  std::cout << "v : verification mode. Verifies results to a third-party program" << std::endl;
  std::cout << "d : debug mode. Displays print for the process of the program" << std::endl;
  std::cout << "s : defines file size. Will read the whole file if not specified" << std::endl;
  std::cout << "f : defines file to read. Will read password.txt if not specified" << std::endl;
  std::cout << "B : defines buffer size in MB. Default is four chunks per buffer" << std::endl;
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

void push_to_verify(struct buffer result, std::vector<std::string> *verify_vec) {
  char hashed_pass[65];
  for (int i = 0; i < result.num_chunks; i++) {
    int c = 0;
    for (int j = 0; j < 32; j++) {
      c += snprintf(hashed_pass + c, 65-c, "%02x", ((unsigned char *) result.chunks[i].data)[j]);
    }
    verify_vec->push_back(hashed_pass);
  }
}

void sha256_fpga(settings *config) {
  DoubleBuffer *double_buffer;
  char *chunk_placement_ptr;
  int written_chunks, lines_read = 0;
  struct buffer result;
  std::vector<std::string> verify_vec;

  double_buffer = new DoubleBuffer();
  std::fstream file;
  file.open(config->filename);

  while (1) {
    // if we want to keep going
    if (config->sopt && config->lines_to_read > lines_read) {
      chunk_placement_ptr = double_buffer->get_chunk()->data;
    } else if (!config->sopt && !file.eof()) {
      chunk_placement_ptr = double_buffer->get_chunk()->data;
    }

    // launch fpga
    if (chunk_placement_ptr == nullptr || file.eof() || config->lines_to_read == lines_read) {
      if (config->dopt) {
        std::cout << "get_chunk() returned nullptr" << std::endl;
        std::cout << "running start_processing().." << std::endl;
      }
      result = double_buffer->start_processing();
      written_chunks = 0;

      if (config->vopt) {
        push_to_verify(result, &verify_vec);
      }
      if (config->dopt) {
        print_result(result);
      }
      if (file.eof() || config->lines_to_read == lines_read) {
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
      lines_read++;
    }
  }
  result = double_buffer->get_last_result();

  if (config->dopt) {
    print_result(result);
  }
  if (config->vopt) {
    push_to_verify(result, &verify_vec);
    verify(verify_vec, config->filename);
  }
  file.close();
  delete double_buffer;
  config->amount_to_process = lines_read * CHUNK_SIZE;
}

void csv_writer(settings *config, std::chrono::duration<double> time) {
  std::ofstream outfile;
  outfile.open(config->outfile, std::ios_base::app);
  outfile << config->amount_to_process/MB << ";" << time.count() << "\n";
  outfile.close();
}

void benchmark(settings *config) {
  std::cout << "Running benchmark..." << std::endl;
  std::chrono::duration<double> time_total;
  auto start = std::chrono::system_clock::now();
  sha256_fpga(config);
  auto end = std::chrono::system_clock::now();
  time_total = end - start;
  csv_writer(config, time_total);
}

int main(int argc, char ** argv) {
  /*Initialization*/
  settings pre_sets;
  pre_settings_init(&pre_sets);
  /*Getopt flags*/
  int c;
  while ((c = getopt(argc, argv, "vbo:dhf:s:B:")) != -1) {
    switch (c) {
      case 'v': {
        pre_sets.vopt = 1;
        break;
      }
      case 'b': {
        pre_sets.bopt = 1;
        break;
      }
      case 'o': {
        pre_sets.oopt = 1;
        pre_sets.outfile = optarg;
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
        pre_sets.amount_to_process = std::stod(optarg);
        break;
      }
      case 'B': {
        pre_sets.buffer_size = std::stod(optarg);
        break;
      }
      case 'h': {
        help();
        std::exit(EXIT_FAILURE);
      }
      default: {
        std::cout << "Input was not recoqnized. use -h to get to the help page" << std::endl;
        std::exit(EXIT_FAILURE);
      }
    }
  }
  pre_settings(&pre_sets);

  if (pre_sets.bopt) {
    benchmark(&pre_sets);
  } else {
    sha256_fpga(&pre_sets);
  }
  return 0;
}
