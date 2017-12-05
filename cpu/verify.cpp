#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "sha256.hpp"
#include "verify.hpp"

using std::cout;
using std::string;
using std::endl;
using std::cout;
using std::vector;
using std::ifstream;

int verify(std::string filename){
  //Get vector with hashed passwords from an extrernal SHA-256 hasher
  vector<string>pass_vec = SHA256_CPU_verify("password.txt");

  ifstream file;
  file.open(filename);
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
