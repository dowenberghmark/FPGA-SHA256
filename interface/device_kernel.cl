#define DATA_SIZE 64
#define NUMBER_ONE 1
#define DATA_TO_TOUCH 32


struct chunk{
  char data[DATA_SIZE];
};

// https://www.xilinx.com/html_docs/xilinx2017_2/sdaccel_doc/topics/pragmas/concept-Intro_to_OpenCL_attributes.html
kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void hashing_kernel(__global struct chunk * __restrict buffer0,
		    __global struct chunk * __restrict buffer1,
		    __global struct chunk * __restrict buffer2,
		    __global struct chunk * __restrict buffer3,
		    const int n_elements,
		    const int active_buf) {
  printf("HELLO FROM FPGA KERNEL 0\n");

  __global struct chunk *buffer;
  if (active_buf == 0) {
    buffer = buffer0;
  } else if (active_buf == 1) {
    buffer = buffer1;
  } else if (active_buf == 2) {
    buffer = buffer2;
  } else if (active_buf == 3) {
    buffer = buffer3;
  }

  //__attribute__((opencl_unroll_hint(n)))
  __attribute__((xcl_pipeline_loop))
    for (int i = 0; i < n_elements; i++) {
      // __attribute__((xcl_pipeline_loop))
      for (int j = 0; j < 16; j++) {
	buffer[i].data[j] = buffer[i].data[j] + NUMBER_ONE;
      }
      //sha2560(input_buffer[i].data, output_buffer[i].data);
    }
}
