#include <stdint.h>
#include <stdio.h>

#define ROTR(x, n) (((x) >> (n)) | ((x) << ((32) - (n)))) //from https://stackoverflow.com/questions/21895604/rotate-right-by-n-only-using-bitwise-operators-in-c

const uint32_t K[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

uint32_t ch(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y)^(~x & z);
}

uint32_t maj(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y)^(x & z)^(y & z);
}

uint32_t sigma0(uint32_t x) {
  return (ROTR(x, 7))^(ROTR(x, 18))^(x >> 3);
}

uint32_t sigma1(uint32_t x) {
  return (ROTR(x, 17))^(ROTR(x, 19))^(x >> 10);
}

uint32_t zigma0(uint32_t x) {
  return (ROTR(x, 2))^(ROTR(x, 13))^(ROTR(x, 22));
}

uint32_t zigma1(uint32_t x) {
  return (ROTR(x, 6))^(ROTR(x, 11))^(ROTR(x, 25));
}

void sha256(char* chunk_address) {
  //Prepare Message Schedule
  uint32_t H0[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
  uint32_t W[64];
  uint32_t a,b,c,d,e,f,g,h,t1,t2,i,j;

  //  __attribute__((opencl_unroll_hint(n)))
  W[0] = ((unsigned char) chunk_address[0] << 24) | ((unsigned char) chunk_address[1] << 16) | ((unsigned char) chunk_address[2] << 8) | ((unsigned char) chunk_address[3]);
  W[1] = ((unsigned char) chunk_address[4] << 24) | ((unsigned char) chunk_address[5] << 16) | ((unsigned char) chunk_address[6] << 8) | ((unsigned char) chunk_address[7]);
  W[2] = ((unsigned char) chunk_address[8] << 24) | ((unsigned char) chunk_address[9] << 16) | ((unsigned char) chunk_address[10] << 8) | ((unsigned char) chunk_address[11]);
  W[3] = ((unsigned char) chunk_address[12] << 24) | ((unsigned char) chunk_address[13] << 16) | ((unsigned char) chunk_address[14] << 8) | ((unsigned char) chunk_address[15]);
  W[4] = ((unsigned char) chunk_address[16] << 24) | ((unsigned char) chunk_address[17] << 16) | ((unsigned char) chunk_address[18] << 8) | ((unsigned char) chunk_address[19]);
  W[5] = ((unsigned char) chunk_address[20] << 24) | ((unsigned char) chunk_address[21] << 16) | ((unsigned char) chunk_address[22] << 8) | ((unsigned char) chunk_address[23]);
  W[6] = ((unsigned char) chunk_address[24] << 24) | ((unsigned char) chunk_address[25] << 16) | ((unsigned char) chunk_address[26] << 8) | ((unsigned char) chunk_address[27]);
  W[7] = ((unsigned char) chunk_address[28] << 24) | ((unsigned char) chunk_address[29] << 16) | ((unsigned char) chunk_address[30] << 8) | ((unsigned char) chunk_address[31]);
  W[8] = ((unsigned char) chunk_address[32] << 24) | ((unsigned char) chunk_address[33] << 16) | ((unsigned char) chunk_address[34] << 8) | ((unsigned char) chunk_address[35]);
  W[9] = ((unsigned char) chunk_address[36] << 24) | ((unsigned char) chunk_address[37] << 16) | ((unsigned char) chunk_address[38] << 8) | ((unsigned char) chunk_address[39]);
  W[10] = ((unsigned char) chunk_address[40] << 24) | ((unsigned char) chunk_address[41] << 16) | ((unsigned char) chunk_address[42] << 8) | ((unsigned char) chunk_address[43]);
  W[11] = ((unsigned char) chunk_address[44] << 24) | ((unsigned char) chunk_address[45] << 16) | ((unsigned char) chunk_address[46] << 8) | ((unsigned char) chunk_address[47]);
  W[12] = ((unsigned char) chunk_address[48] << 24) | ((unsigned char) chunk_address[49] << 16) | ((unsigned char) chunk_address[50] << 8) | ((unsigned char) chunk_address[51]);
  W[13] = ((unsigned char) chunk_address[52] << 24) | ((unsigned char) chunk_address[53] << 16) | ((unsigned char) chunk_address[54] << 8) | ((unsigned char) chunk_address[55]);
  W[14] = ((unsigned char) chunk_address[56] << 24) | ((unsigned char) chunk_address[57] << 16) | ((unsigned char) chunk_address[58] << 8) | ((unsigned char) chunk_address[59]);
  W[15] = ((unsigned char) chunk_address[60] << 24) | ((unsigned char) chunk_address[61] << 16) | ((unsigned char) chunk_address[62] << 8) | ((unsigned char) chunk_address[63]);


  __attribute__((xcl_pipeline_loop))
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
  __attribute__((xcl_pipeline_loop))
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

  printf("H0 after loop: \n");
  for (int i = 0; i < 8; i++) {
    printf("%x\n", H0[i]);
  }



  //Store hash in input buffer
  //__attribute__((opencl_unroll_hint(n)))

  for (int i = 0; i < 8; i++) {
    ((uint32_t *) chunk_address)[i] = (((unsigned char *) H0)[i*4] << 24) | (((unsigned char *) H0)[i*4+1] << 16) | (((unsigned char *) H0)[i*4+2] << 8) | (((unsigned char *) H0)[i*4+3]);
  }
}
