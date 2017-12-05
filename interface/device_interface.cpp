// device interface for connecting cpu to fpga using opencl code.
// initialization based on SDaccel hello world program

#include "device_interface.hpp"
#include "defs.hpp"
#include "xcl2.hpp"


DeviceInterface::DeviceInterface(struct chunk *buffer0, struct chunk *buffer1) {
  first_flag = 1;
  // The get_xil_devices will return vector of Xilinx Devices
  std::vector<cl::Device> devices = xcl::get_xil_devices();
  cl::Device device = devices[0];

  //Creating Context and Command Queue for selected Device
  cl::Context context(device);
  q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE);
  std::string device_name = device.getInfo<CL_DEVICE_NAME>();
  std::cout << "Found Device=" << device_name.c_str() << std::endl;

  // import_binary() command will find the OpenCL binary file created using the
  // xocc compiler load into OpenCL Binary and return as Binaries
  // OpenCL and it can contain many functions which can be executed on the
  // device.
  std::string binaryFile = xcl::find_binary_file(device_name,"device_kernel");
  cl::Program::Binaries bins = xcl::import_binary_file(binaryFile);
  devices.resize(1);
  program = cl::Program(context, devices, bins);


  cl_mem_ext_ptr_t bank_buffer0, bank_buffer1;  // Declaring two extensions for both buffers
  bank_buffer0.flags  = XCL_MEM_DDR_BANK0; // Specify Bank0 Memory for input memory
  bank_buffer1.flags = XCL_MEM_DDR_BANK1; // Specify Bank1 Memory for output Memory
  bank_buffer0.obj   = buffer0; 
  bank_buffer1.obj  = buffer1; // Setting Obj and Param to Zero
  bank_buffer0.param = 0 ; bank_buffer1.param = 0; 

  // This call will extract a kernel out of the program we loaded in the
  // previous line. A kernel is an OpenCL function that is executed on the
  // FPGA. This function is defined in the interface/device_kernel.cl file.
  // krnl_sha = cl::Kernel(program, "fpga_sha");

  ocl_bufs[0] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX, BUFFER_SIZE, &bank_buffer0);
  ocl_bufs[1] = cl::Buffer(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX, BUFFER_SIZE, &bank_buffer1);

  host_bufs[0].chunks = buffer0;
  host_bufs[1].chunks = buffer1;

  // This call will extract a kernel out of the program we loaded in the
  // previous line. A kernel is an OpenCL function that is executed on the
  // FPGA. This function is defined in the interface/device_kernel.cl file.
  krnl_sha = cl::Kernel(program, "device_kernel");
}

void DeviceInterface::run_fpga(int num_chunks, int active_buf) {
  
  // Initialize for profiling
  uint64_t nstimestart, nstimeend;
  cl::Event event_kernel, event_w_buffer, event_r_buffer;


  // The data will be be transferred from system memory over PCIe to the FPGA on-board
  // DDR memory. blocking.
  q.enqueueWriteBuffer(ocl_bufs[active_buf], CL_TRUE, 0, num_chunks*CHUNK_SIZE, host_bufs[active_buf].chunks, NULL, &event_w_buffer);

  // Profiling write to buffer.
  event_w_buffer.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_START,&nstimestart);
  event_w_buffer.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_END,&nstimeend);
  auto duration_nanosec = nstimeend-nstimestart;
  std::cout << " **** Profile write to buffer is " << (duration_nanosec * (1.0e-6) ) << " ms **** " << std::endl;

  
  host_bufs[active_buf].num_chunks = num_chunks;

  //set the kernel Arguments
  int narg=0;
  //  krnl_sha.setArg(narg++, ocl_bufs[active_buf]);
  krnl_sha.setArg(narg++, ocl_bufs[0]);
  krnl_sha.setArg(narg++, ocl_bufs[1]);
  krnl_sha.setArg(narg++, active_buf);

  krnl_sha.setArg(narg++, num_chunks);

  //Launch the Kernel
  q.enqueueTask(krnl_sha, NULL, &event_kernel);
  event_kernel.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_START,&nstimestart);
  event_kernel.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_END,&nstimeend);
  duration_nanosec = nstimeend-nstimestart;
  std::cout << " **** Profile kernel computation is " << (duration_nanosec * (1.0e-6) ) << " ms **** " << std::endl;


  // The result of the previous kernel execution will need to be retrieved in
  // order to view the results. This call will write the data from the
  // buffer_result cl_mem object to the source_results vector
  // first_flag causes us to not read result buffer first time
  if (!first_flag) {
    // blocking.
    q.enqueueReadBuffer(ocl_bufs[1 - active_buf], CL_TRUE, 0, host_bufs[1 - active_buf].num_chunks*CHUNK_SIZE, host_bufs[1 - active_buf].chunks, NULL, &event_r_buffer);
    event_r_buffer.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_START,&nstimestart);
    event_r_buffer.getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_END,&nstimeend);
    duration_nanosec = nstimeend-nstimestart;
    std::cout << " **** Profile read buffer is " << (duration_nanosec * (1.0e-6) ) << " ms **** " << std::endl;

    host_bufs[1 - active_buf].num_chunks = 0;
  } else {
    first_flag = 0;
  }
  q.finish();
}
