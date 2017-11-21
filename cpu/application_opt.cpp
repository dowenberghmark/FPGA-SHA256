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
  char *s;
    while(!file.eof()){
      file >> element;
      s = &element[0u];

/*appendar 1 och k-times 0*/
  unsigned char one = 0x01;
  unsigned char zero = 0x00;
 int i = strlen(element);
 int j = i;
 int k = 0;
if (strlen(element) < 56){
    element[i++] = one;
    while (i < 56){
     element[i++] = 0x00;
    }
}

/*appendar strlen(element)*8 i binär form */
while(i < 64){
  cout << "strlen is: " << j << endl;
  for(int k = 0;k<8;k++){
    element[i++] = bitset<8>(j*8)[k];
  }
}


}

  for(int i=0;i<sizeof(element);i++){
    //out << "print s_pre: " << element[i] << endl;
    printf ("%d: %x \n",i,element[i]);
  }
  cout << "print eleement: " << element << endl;
  cout << "print strlen(element): " << strlen(element) << endl;
  cout << "print sizeof(element): " << sizeof(element) << endl;

//test
   return 0;
}