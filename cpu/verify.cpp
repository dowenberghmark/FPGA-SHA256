#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "sha256.hpp"

using namespace std;


int main() {

	//Get vector with hashed passwords from an extrernal SHA-256 hasher
	vector<string>pass_vec = SHA256_CPU_vec("password.txt");

	ifstream file;
	//Uses dummie file until we get our own results
  file.open("hashed_passwords.txt");
  string element;
  int counter = 0;
  while(!file.eof()){
    file >> element;
    if (element.compare(pass_vec[counter]) != 0){
      cout << "Line " << counter+1 << " is incorrect" << endl;
    }
    counter++;
  }
  cout << "Comparison completed" << endl;
  return 0;
}