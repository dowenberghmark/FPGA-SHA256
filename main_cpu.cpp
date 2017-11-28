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


using namespace std;

#define uchar unsigned char
#define uint unsigned int
int debug_flag, compile_flag, size_in_bytes;

void pre_process(char* element){
/*appends 1 after string*/
  int num = 1;
  unsigned char one;
    if(*(char *)&num == 1){   // Little-Endian
      one = 1 << 7;
    } else{  // Big-Endian
      one = 1;
    }
    int strLen = strlen(element);
    element[strLen++] = one;
    strLen--;
    unsigned char bytes[4];
    char l_append;
    l_append = strLen*8;

    if(*(char *)&num == 1){  // if Little-Endian
      l_append = strLen*8;
      bytes[0] = (l_append >> 24) & 0xFF;
      bytes[1] = (l_append >> 16) & 0xFF;
      bytes[2] = (l_append >> 8) & 0xFF;
      bytes[3] = l_append & 0xFF;
      element[60] = bytes[0];
      element[61] = bytes[1];
      element[62] = bytes[2];
      element[63] = bytes[3];    
    } else{} // Big-Endian
  }

  int main(int argc, char ** argv){
    int c;

    int bopt = 0, dopt = 0, sopt = 0;
    int fopt = 0;
    char *fvalue = NULL;
    int svalue;

    string filename;
    int filesize;
    int lines_to_read;



    while ((c = getopt (argc, argv, "b,d,f:s:")) != -1){
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

        default:
        abort ();
      }
    }



    cout << "======================== PRE SETTINGS ==========================" << endl;
    if(bopt == 1){
      cout << "bopt is on" << endl;
      
    }

    if(dopt == 1){
      cout << "dopt is on" << endl;
    }


    if(fopt == 1){ //filename flag
      filename = fvalue;
      cout << "filename: " << filename << endl;
    } else{
      filename = "password.txt";
      cout << "filename: " << filename << endl;
    }

    if(sopt == 1){ //size flag
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
    chrono::duration<double> time_total;

    auto start_preprocess = chrono::system_clock::now();
    auto end_preprocess = chrono::system_clock::now();

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
    our_double_buffer = new DoubleBuffer(dram_path);
    auto start = std::chrono::system_clock::now();  //benchmark variable
    while(!file.eof()){

      memset(element,0,64);
      file >> element;
      chunk_placement_ptr = our_double_buffer->get_chunk();

      if(MODE == LOCAL){

        cout << "reading string: " << element << endl;
      }

      if(chunk_placement_ptr == nullptr){
        if (MODE == LOCAL){
          cout << "get_chunk() returned nullptr" << endl;
          cout << "running start_processing() & get_result().." << endl;
        }

        our_double_buffer->start_processing();
        our_double_buffer->get_result();
        chunk_placement_ptr = our_double_buffer->get_chunk();
      }


      start_preprocess = chrono::system_clock::now();
      // should always run this
      pre_process(element);
      memcpy(chunk_placement_ptr,element,sizeof(element));

      end_preprocess = chrono::system_clock::now();
      time_preprocess += end_preprocess - start_preprocess;
      lines_to_read--;
      if (lines_to_read == 0){
        break;
      }

    }

    

    
    our_double_buffer->done();
    auto end = std::chrono::system_clock::now();
    file.close();

    time_total = end - start;
     if(MODE == AWS){ // benchmark flag
      cout << "Running sha256 CPU program..." << endl;
      auto start = std::chrono::system_clock::now();
      SHA256_CPU_benchmark(filename);
      auto end = std::chrono::system_clock::now();
      chrono::duration<double> cpu_program_time = end - start;
      cout << endl;
      cout << "====================== BENCHMARK RESULTS =======================" << endl;
      cout << "fpga pre-process time: " << time_preprocess.count() << "s" << endl;
      cout << "fpga program time: "<< time_total.count() << "s" << endl;
      cout << endl;
      cout << "CPU sha256 program time: " <<  cpu_program_time.count() << "s" << endl;
      cout << "================================================================" << endl;
    }

    if(MODE == LOCAL){

    }
    return 0;
  }