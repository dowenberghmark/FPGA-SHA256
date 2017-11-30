#define DATA_SIZE 64
#define NUMBER_ONE 1
#define DATA_TO_TOUCH 32

struct chunk{
  char data[64];
};

// https://www.xilinx.com/html_docs/xilinx2017_2/sdaccel_doc/topics/pragmas/concept-Intro_to_OpenCL_attributes.html
kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void fpga_sha(global struct chunk *chunk_buffer, const int n_elements) {
  printf("HELLO FROM FPGA\n");
  //__attribute__((xcl_pipeline_loop))
  __attribute__((opencl_unroll_hint(n)))
    for (int i = 0; i < n_elements; i++) {
      // __attribute__((xcl_pipeline_loop))
      __attribute__((opencl_unroll_hint(n)))
	for (int j = 0; j < DATA_TO_TOUCH; j++) {
	  chunk_buffer[i].data[j] = chunk_buffer[i].data[j] + NUMBER_ONE;
	}
    }
}
