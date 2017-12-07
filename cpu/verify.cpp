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

int verify(std::vector<std::string> fpga_hash_vec){
  //Get vector with hashed passwords from an extrernal SHA-256 hasher
  vector<string>ext_hash_vec = SHA256_CPU_verify("random_passwords.txt");
  int error_row;

  for(auto i = fpga_hash_vec.begin(); i != fpga_hash_vec.end(); ++i){
    string s = fpga_hash_vec[i];
    if (s.compare(ext_hash_vec[i]) != 0){
      error_row = i+1;
      cout << "Line " << error_row << " is incorrect" << endl;
    }
  }

  cout << "Comparison completed" << endl;
  return 0;
}
