#define DATA_SIZE 64
#define NUMBER_ONE 1
#define DATA_TO_TOUCH 32

#define LOC_BUF_SIZE 4
#define NUM_CHUNKS 16

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

void sha256(__local char *buffer) {

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
  for (int i = 0;  i < 16; i++) {
    W[i] = ((__local unsigned char) buffer[j] << 24) | ((__local unsigned char) buffer[j+1] << 16) | ((__local unsigned char) buffer[j+2] << 8) | ((__local unsigned char) buffer[j+3]);
    j += 4;
  }

  for (int i = 16; i < 64; i+=2) {
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
  for (int i = 0; i < 32; i+=4) {
    ((__local char *) buffer)[i+3] = (((unsigned char *) H0)[i]);
    ((__local char *) buffer)[i+2] = (((unsigned char *) H0)[i+1]);
    ((__local char *) buffer)[i+1] = (((unsigned char *) H0)[i+2]);
    ((__local char *) buffer)[i] = (((unsigned char *) H0)[i+3]);
  }
}


kernel __attribute__((reqd_work_group_size(1, 1, 1)))
__attribute__((xcl_dataflow))
void hashing_kernel(__global struct chunk * __restrict buffer0,
		    __global struct chunk * __restrict buffer1,
		    const int active_buf) {
  printf("HELLO FROM FPGA KERNEL\n");

  __global struct chunk *global_buf;
  if (active_buf == 0) {
    global_buf = buffer0;
  } else if (active_buf == 1) {
    global_buf = buffer1;
  }
  __local struct chunk local_buf[LOC_BUF_SIZE];

  __attribute__ ((xcl_pipeline_loop))
  for (int chunk = 0; chunk < NUM_CHUNKS; chunk+=LOC_BUF_SIZE) {
    for (int i = 0; i < LOC_BUF_SIZE; i++) {
      for (int j = 0; j < 64; j++) {
	local_buf[i].data[j] = global_buf[i + chunk].data[j];
      }
    }
    for (int i = 0; i < LOC_BUF_SIZE; i++) {
      sha256(local_buf[i].data);
    }
    for (int i = 0; i < LOC_BUF_SIZE; i++) {
      for (int j = 0; j < 64; j++) {
	global_buf[i + chunk].data[j] = local_buf[i].data[j];
      }
    }
  }

  /*
  __attribute__((xcl_pipeline_loop))
  for (int i = 0; i < 16; i+=1) {
    sha256(buffer[i].data);
  }
  */
}
