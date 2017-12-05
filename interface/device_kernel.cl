#define DATA_SIZE 64
#define NUMBER_ONE 1
#define DATA_TO_TOUCH 32

struct chunk{
  char data[64];
};

// https://www.xilinx.com/html_docs/xilinx2017_2/sdaccel_doc/topics/pragmas/concept-Intro_to_OpenCL_attributes.html
kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void device_kernel(__global struct chunk *chunk_buffer0, __global struct chunk *chunk_buffer1, const int which, const int n_elements) {
  printf("HELLO FROM FPGA\n");
                           
  //__attribute__((xcl_pipeline_loop))
  __attribute__((opencl_unroll_hint(n)))
      for (int i = 0; i < n_elements; i++) {
        // __attribute__((xcl_pipeline_loop))
        __attribute__((opencl_unroll_hint(n)))
            for (int j = 0; j < DATA_TO_TOUCH; j++) {
              // chunk_buffer[i].data[j] = chunk_buffer[i].data[j] + NUMBER_ONE;
              if(!which){
                printf("%c", chunk_buffer0[i].data[j]);
                chunk_buffer0[i].data[j] = chunk_buffer0[i].data[j] + NUMBER_ONE;
                printf("%c", chunk_buffer0[i].data[j]);
              }
              else{
                printf("%c", chunk_buffer1[i].data[j]);
                chunk_buffer1[i].data[j] = chunk_buffer1[i].data[j] + NUMBER_ONE;
                printf("%c", chunk_buffer1[i].data[j]);
              }
            }
      }  
}
