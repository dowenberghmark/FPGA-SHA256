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
void pre_process(){
/*ÖPPNAR PASSWORD.TXT*/
  fstream file;
  file.open("password.txt");
  char element[64];
  while(!file.eof()){
    memset(element,0,64); //fills block memory with 0x00
    file >> element;


/*appends 1 after string*/
    int num = 1;
    unsigned char one;
    if(*(char *)&num == 1){   // Little-Endian
      one = 1 << 7;
    }
    else{  // Big-Endian
      one = 1;
    }
    int strLen = strlen(element);
    element[strLen++] = one;
    strLen--;


/*appends strlen(element)*8 to the last byte. Don't think it needs to be big/little endian dependent*/
    int i=55;
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
    }
    else{  // Big-Endian
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
    /*vänta på att få skicka chunck*/
    while(true){

    }
  }

}

int main() {

pre_process();

//test
  return 0;
}