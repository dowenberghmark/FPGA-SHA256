#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <chrono>
#include <bitset>
#include <iomanip>
#include "sha256.hpp"

using namespace std;

#define uchar unsigned char
#define uint unsigned int

int main() {
/*ÖPPNAR PASSWORD.TXT*/
  fstream file;
  file.open("password.txt");
  char element[64];

  while(!file.eof()){

    memset(element,0x00,64); //fills block memory with 0x00
    file >> element;


/*appends 1 after string*/
    unsigned char one = 1 << 7;

    int strLen = strlen(element);
    element[strLen++] = one;
    strLen--;


/*appendar strlen(element)*8 i binär form */
    int i=63;
    int num = 1;
    
    if(*(char *)&num == 1){  // if Little-Endian
      /*
      for(int k=0; k<8; k++){
       element[i] = bitset<64>(strLen*8)[k];
       printf ("%d: %x \n",k,element[i]);
       i--;
       */
      element[63] = strLen*8;
     
     

   }else{  // Big-Endian
    i = 55;
    for(int k=0; k<8; k++){
     element[i] = bitset<8>(strLen*8)[k];
     printf ("%d: %x \n",k,element[i]);
     i++;
   }
 }
}

for(int i=0;i<sizeof(element);i++){
  printf ("%d: %x \n",i,element[i]);
}

int counter = 0;
for(int i=0;i<64; i++){
  bitset<8> x(element[i]);
  if(i == 57){
    cout << "64 bits from here:" << endl;
  }
  cout << x;
  counter += 8;
  cout << ", " << endl;
}



//test
return 0;
}