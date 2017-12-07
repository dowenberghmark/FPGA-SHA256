#define DATA_SIZE 64
#define NUMBER_ONE 1
#define DATA_TO_TOUCH 32

struct chunk{
  char data[DATA_SIZE];
};

// https://www.xilinx.com/html_docs/xilinx2017_2/sdaccel_doc/topics/pragmas/concept-Intro_to_OpenCL_attributes.html
kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void hashing_kernel0(__global struct chunk * __restrict input_buffer,
		     __global struct chunk * __restrict output_buffer,
		     const int n_elements,
		     const int active_buf) {
  printf("HELLO FROM FPGA KERNEL 0\n");
  //__attribute__((opencl_unroll_hint(n)))
  __attribute__((xcl_pipeline_loop))
    for (int i = 0; i < n_elements; i++) {
      // __attribute__((xcl_pipeline_loop))
      __attribute__((opencl_unroll_hint))
	for (int j = 0; j < DATA_TO_TOUCH; j++) {
	  // if (active_buf == 0) {
	  char tmp = input_buffer[i].data[j] + NUMBER_ONE;
	  output_buffer[i].data[j] = tmp;
	}
    }
}

kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void hashing_kernel1(__global struct chunk * __restrict input_buffer,
		     __global struct chunk * __restrict output_buffer,
		     const int n_elements,
		     const int active_buf) {
  printf("HELLO FROM FPGA KERNEL 1\n");
  //__attribute__((opencl_unroll_hint(n)))
  __attribute__((xcl_pipeline_loop))
    for (int i = 0; i < n_elements; i++) {
      // __attribute__((xcl_pipeline_loop))
      __attribute__((opencl_unroll_hint))
	for (int j = 0; j < DATA_TO_TOUCH; j++) {
	  // if (active_buf == 0) {
	  char tmp = input_buffer[i].data[j] + NUMBER_ONE;
	  output_buffer[i].data[j] = tmp;
	}
    }
}
