#define DATA_SIZE 64
#define NUMBER_ONE 1
#define DATA_TO_TOUCH 32

#define ROTR(x, n) (((x) >> (n)) | ((x) << ((32) - (n)))) //from https://stackoverflow.com/questions/21895604/rotate-right-by-n-only-using-bitwise-operators-in-c

// https://www.xilinx.com/html_docs/xilinx2017_2/sdaccel_doc/topics/pragmas/concept-Intro_to_OpenCL_attributes.html

struct chunk{
  char data[DATA_SIZE];
};


//auxiliary functions for the hashing algorithm
uint ch(uint x, uint y, uint z) {
  return (x & y)^(~x & z);
}

uint maj(uint x, uint y, uint z) {
  return (x & y)^(x & z)^(y & z);
}

uint sigma0(uint x) {
  return (ROTR(x, 7))^(ROTR(x, 18))^(x >> 3);
}

uint sigma1(uint x) {
  return (ROTR(x, 17))^(ROTR(x, 19))^(x >> 10);
}

uint zigma0(uint x) {
  return (ROTR(x, 2))^(ROTR(x, 13))^(ROTR(x, 22));
}

uint zigma1(uint x) {
  return (ROTR(x, 6))^(ROTR(x, 11))^(ROTR(x, 25));
}

void sha256(__global char *buffer) {

  __private uint K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
  };


  //Prepare Message Schedule
  uint H0[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
  uint W[64];
  uint a,b,c,d,e,f,g,h,t1,t2;


  int j = 0;
  //  __attribute__((opencl_unroll_hint(n)))
  for (int i = 0;  i < 16; i++) {
    W[i] = ((__global unsigned char) buffer[j] << 24) | ((__global unsigned char) buffer[j+1] << 16) | ((__global unsigned char) buffer[j+2] << 8) | ((__global unsigned char) buffer[j+3]);
    j += 4;
  }

  //__attribute__((opencl_unroll_hint(n)))
  for (int i = 16; i < 64; i++) {
    W[i] = sigma1(W[i-2]) + W[i-7] + sigma0(W[i-15]) + W[i-16];
  }

  /*
  W[0] = ((unsigned char) buffer[0] << 24) | ((unsigned char) buffer[1] << 16) | ((unsigned char) buffer[2] << 8) | ((unsigned char) buffer[3]);
  W[1] = ((unsigned char) buffer[4] << 24) | ((unsigned char) buffer[5] << 16) | ((unsigned char) buffer[6] << 8) | ((unsigned char) buffer[7]);
  W[2] = ((unsigned char) buffer[8] << 24) | ((unsigned char) buffer[9] << 16) | ((unsigned char) buffer[10] << 8) | ((unsigned char) buffer[11]);
  W[3] = ((unsigned char) buffer[12] << 24) | ((unsigned char) buffer[13] << 16) | ((unsigned char) buffer[14] << 8) | ((unsigned char) buffer[15]);
  W[4] = ((unsigned char) buffer[16] << 24) | ((unsigned char) buffer[17] << 16) | ((unsigned char) buffer[18] << 8) | ((unsigned char) buffer[19]);
  W[5] = ((unsigned char) buffer[20] << 24) | ((unsigned char) buffer[21] << 16) | ((unsigned char) buffer[22] << 8) | ((unsigned char) buffer[23]);
  W[6] = ((unsigned char) buffer[24] << 24) | ((unsigned char) buffer[25] << 16) | ((unsigned char) buffer[26] << 8) | ((unsigned char) buffer[27]);
  W[7] = ((unsigned char) buffer[28] << 24) | ((unsigned char) buffer[29] << 16) | ((unsigned char) buffer[30] << 8) | ((unsigned char) buffer[31]);
  W[8] = ((unsigned char) buffer[32] << 24) | ((unsigned char) buffer[33] << 16) | ((unsigned char) buffer[34] << 8) | ((unsigned char) buffer[35]);
  W[9] = ((unsigned char) buffer[36] << 24) | ((unsigned char) buffer[37] << 16) | ((unsigned char) buffer[38] << 8) | ((unsigned char) buffer[39]);
  W[10] = ((unsigned char) buffer[40] << 24) | ((unsigned char) buffer[41] << 16) | ((unsigned char) buffer[42] << 8) | ((unsigned char) buffer[43]);
  W[11] = ((unsigned char) buffer[44] << 24) | ((unsigned char) buffer[45] << 16) | ((unsigned char) buffer[46] << 8) | ((unsigned char) buffer[47]);
  W[12] = ((unsigned char) buffer[48] << 24) | ((unsigned char) buffer[49] << 16) | ((unsigned char) buffer[50] << 8) | ((unsigned char) buffer[51]);
  W[13] = ((unsigned char) buffer[52] << 24) | ((unsigned char) buffer[53] << 16) | ((unsigned char) buffer[54] << 8) | ((unsigned char) buffer[55]);
  W[14] = ((unsigned char) buffer[56] << 24) | ((unsigned char) buffer[57] << 16) | ((unsigned char) buffer[58] << 8) | ((unsigned char) buffer[59]);
  W[15] = ((unsigned char) buffer[60] << 24) | ((unsigned char) buffer[61] << 16) | ((unsigned char) buffer[62] << 8) | ((unsigned char) buffer[63]);
  */


  //__attribute__((xcl_pipeline_loop))
  for (int i = 16; i < 32; i+=2) {
    W[i] = sigma1(W[i-2]) + W[i-7] + sigma0(W[i-15]) + W[i-16];
    W[i+1] = sigma1(W[i-1]) + W[i-6] + sigma0(W[i-14]) + W[i-15];
  }


  //Initialize working variables
  a = H0[0];
  b = H0[1];
  c = H0[2];
  d = H0[3];
  e = H0[4];
  f = H0[5];
  g = H0[6];
  h = H0[7];

  //Compute Hash
  //__attribute__((xcl_pipeline_loop))
  for (int i = 0; i < 64; i++) {
    t1 = h + zigma1(e) + ch(e, f, g) + K[i] + W[i];
    t2 = zigma0(a) + maj(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  H0[0] = H0[0] + a;
  H0[1] = H0[1] + b;
  H0[2] = H0[2] + c;
  H0[3] = H0[3] + d;
  H0[4] = H0[4] + e;
  H0[5] = H0[5] + f;
  H0[6] = H0[6] + g;
  H0[7] = H0[7] + h;

  //Store hash in input buffer
   //__attribute__((opencl_unroll_hint(n)))

  for (int i = 0; i < 8; i++) {
    ((__global uint *) buffer)[i] = (((unsigned char *) H0)[i*4] << 24) | (((unsigned char *) H0)[i*4+1] << 16) | (((unsigned char *) H0)[i*4+2] << 8) | (((unsigned char *) H0)[i*4+3]);
  }
  /*
  ((__global uint *) buffer)[0] = (((unsigned char *) H0)[0] << 24) | (((unsigned char *) H0)[1] << 16) | (((unsigned char *) H0)[2] << 8) | (((unsigned char *) H0)[3]);
  ((__global uint *) buffer)[1] = (((unsigned char *) H0)[4] << 24) | (((unsigned char *) H0)[5] << 16) | (((unsigned char *) H0)[6] << 8) | (((unsigned char *) H0)[7]);
  ((__global uint *) buffer)[2] = (((unsigned char *) H0)[8] << 24) | (((unsigned char *) H0)[9] << 16) | (((unsigned char *) H0)[10] << 8) | (((unsigned char *) H0)[11]);
  ((__global uint *) buffer)[3] = (((unsigned char *) H0)[12] << 24) | (((unsigned char *) H0)[13] << 16) | (((unsigned char *) H0)[14] << 8) | (((unsigned char *) H0)[15]);
  ((__global uint *) buffer)[4] = (((unsigned char *) H0)[16] << 24) | (((unsigned char *) H0)[17] << 16) | (((unsigned char *) H0)[18] << 8) | (((unsigned char *) H0)[19]);
  ((__global uint *) buffer)[5] = (((unsigned char *) H0)[20] << 24) | (((unsigned char *) H0)[21] << 16) | (((unsigned char *) H0)[22] << 8) | (((unsigned char *) H0)[23]);
  ((__global uint *) buffer)[6] = (((unsigned char *) H0)[24] << 24) | (((unsigned char *) H0)[25] << 16) | (((unsigned char *) H0)[26] << 8) | (((unsigned char *) H0)[27]);
  ((__global uint *) buffer)[7] = (((unsigned char *) H0)[28] << 24) | (((unsigned char *) H0)[29] << 16) | (((unsigned char *) H0)[30] << 8) | (((unsigned char *) H0)[31]);*/
}


kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void hashing_kernel(__global struct chunk * __restrict buffer0,
		    __global struct chunk * __restrict buffer1,
		    const int n_elements,
		    const int active_buf) {
  printf("HELLO FROM FPGA KERNEL\n");

  __global struct chunk *buffer;
  if (active_buf == 0) {
    buffer = buffer0;
  } else if (active_buf == 1) {
    buffer = buffer1;
  }

  __attribute__((xcl_pipeline_loop))
  for (int i = 0; i < n_elements; i+=4) {
    sha256(buffer[i].data);
    sha256(buffer[i+1].data);
    sha256(buffer[i+2].data);
    sha256(buffer[i+3].data);
  }
}
