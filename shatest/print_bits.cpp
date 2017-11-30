#include <bitset>
#include<iostream>
#include <stdio.h>

using namespace std;
#define uchar unsigned char

void print_bits(char* element, int n){
	for(int i=0; i<n;i++){
		cout << "PRINTING ELEMENT IN BITS: " << bitset<8>(element[i]);
		printf("  %x \n",(uchar)element[i]);
	}
}