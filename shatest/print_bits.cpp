#include <bitset>
#include<iostream>
#include <stdio.h>

#define uchar unsigned char

void print_bits(char* element, int n){
	for(int i=0; i<n;i++){
		std::cout << "PRINTING ELEMENT IN BITS: " << std::bitset<8>(element[i]);
		printf("  %x \n",(uchar)element[i]);
	}
}