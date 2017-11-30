#include <iostream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <chrono>
#include <ctime>
#include <bitset>
#include <iomanip>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <cmath>

#include "interface/double_buffer.hpp"
#include "interface/defs.hpp"
#include "cpu/sha256.hpp"
#include "cpu/sha_preprocess.hpp"

using namespace std;

int main(int argc, char ** argv) {
  int c;

  int bopt = 0, dopt = 0, sopt = 0;
  int fopt = 0;
  char *fvalue = NULL;
  int svalue;

  string filename;
  int filesize;
  int lines_to_read;

  while ((c = getopt (argc, argv, "d,h,f:s:")) != -1) {
      //int this_option_optind = optind ? optind : 1;
    switch (c) {

      case 'b':
      bopt = 1;
      break;

      case 'd':
      dopt = 1;
      break;

      case 'f':
      fopt = 1;
      fvalue = optarg;
      break;

      case 's':
      sopt = 1;
      svalue = stoi(optarg);
      break;

      case 'h':
      cout << "=============================== HELP PAGE ===================================" << endl;
      cout << "usage: ./main [-d] [-s size in MB] [-f filepath]" << endl;
      cout << "d : debug mode. Displays print for the process of the program" << endl;
      cout << "s : define file size. Will read the whole file if not specified" << endl;
      cout << "f : define file to read. Will read password.txt if not specified" << endl;
      cout << "h : help page" << endl;
      cout << "==============================================================================" << endl;
      abort ();

      default:
      abort ();
    }
  }

  cout << "======================== PRE SETTINGS ==========================" << endl;
  if(dopt == 1) {
    cout << "debug mode is on" << endl;
  }
    if(fopt == 1) { //filename flag
      filename = fvalue;
      cout << "filename: " << filename << endl;
    } else{
      filename = "password.txt";
      cout << "filename: " << filename << endl;
    }

    if(sopt == 1) { //size flag
      filesize = svalue * 1000;
      lines_to_read = trunc(filesize/64);
      cout << "size: " << filesize << "MB" << endl; 
      cout << "lines_to_read: " << lines_to_read << endl;
    } else{
      cout << "size: whole file will be read" << endl;
    }
    cout << endl;
    
    DoubleBuffer *our_double_buffer;
    char *chunk_placement_ptr;
    char const *dram_path;
    char element[64];
    chrono::duration<double> time_preprocess;
    chrono::duration<double> time_buffer_processing;
    chrono::duration<double> time_retrive_result;
    chrono::duration<double> time_total;

    auto start_preprocess = chrono::system_clock::now();
    auto end_preprocess = chrono::system_clock::now();
    auto start_buffer_processing = chrono::system_clock::now();
    auto end_buffer_processing = chrono::system_clock::now();
    auto start_retrive_result = chrono::system_clock::now();
    auto end_retrive_result = chrono::system_clock::now();


    fstream file;
    file.open(filename);

    if (MODE == LOCAL) {
      dram_path = "dram.hex";
      cout << "LOCAL MODE" << endl;
    } else if (MODE == AWS) {
      dram_path = "/dev/edma0_queue_0";
      cout << "AWS MODE" << endl;
    }  
    cout << "reading from dram: " << dram_path << endl;
    cout << "================================================================" << endl;

    /*Initialization*/
    our_double_buffer = new DoubleBuffer(dram_path);
    auto start = std::chrono::system_clock::now();  //benchmark variable
    
    /*start reading file*/
    while(!file.eof()) {
      memset(element,0,64);
      file >> element;
      chunk_placement_ptr = our_double_buffer->get_chunk();

      if(dopt == 1) {
        cout << "reading string: " << element << endl;
      }

      if(chunk_placement_ptr == nullptr) {
        if (dopt == 1) {
          cout << "get_chunk() returned nullptr" << endl;
          cout << "running start_processing() & get_result().." << endl;
        }
        start_buffer_processing = chrono::system_clock::now();
        our_double_buffer->start_processing();
        end_buffer_processing = chrono::system_clock::now();
        time_buffer_processing += end_buffer_processing - start_buffer_processing;

        start_retrive_result = chrono::system_clock::now();
        our_double_buffer->get_result();
        end_retrive_result = chrono::system_clock::now();
        time_retrive_result += end_retrive_result - start_retrive_result; 
        if(dopt == 1) {
          cout << "get_chunk() returned: " << &chunk_placement_ptr << endl;
        }
        chunk_placement_ptr = our_double_buffer->get_chunk();
      }


      start_preprocess = chrono::system_clock::now();
      // should always run this
      pre_process(element);
      memcpy(chunk_placement_ptr,element,sizeof(element));

      end_preprocess = chrono::system_clock::now();
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
    if(MODE == AWS){ 
      cout << "Running sha256 CPU program..." << endl;
      auto start = std::chrono::system_clock::now();
      SHA256_CPU_benchmark(filename);
      auto end = std::chrono::system_clock::now();
      chrono::duration<double> cpu_program_time = end - start;
      cout << endl;
      cout << "====================== BENCHMARK RESULTS =======================" << endl;
      cout << "FPGA sha256 program time: "<< time_total.count() << "s" << endl;
      cout << endl;
      cout << "CPU sha256 program time: " <<  cpu_program_time.count() << "s" << endl;
      cout << "================================================================" << endl;
    }

    if(MODE == LOCAL || dopt == 1) {
      cout << "====================== DEBUG TIME RESULTS =======================" << endl;
      cout << "pre_process() time: " << time_preprocess.count() << "s" << endl;
      cout << "start_processing() function time: " << time_buffer_processing.count() << "s" << endl;
      cout << "get_result() function time: " << time_retrive_result.count() << "s" << endl;
      cout << "total program time: "<< time_total.count() << "s" << endl;
      cout << "================================================================" << endl;
    }
    return 0;
  }