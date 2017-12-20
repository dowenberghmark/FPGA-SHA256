#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "host_sha256.hpp"
#include "verify.hpp"

using std::cout;
using std::string;
using std::endl;
using std::vector;
using std::ifstream;

int verify(vector<string> fpga_hash_vec, string filename) {
  // Get vector with hashed passwords from an extrernal SHA-256 hasher
  cout << "Trying to verify" << endl;
  vector<string>ext_hash_vec = host_sha256_verify(filename);
  int errors = 0;

  for (unsigned i=0; i < fpga_hash_vec.size(); i++) {
    if (fpga_hash_vec[i].compare(ext_hash_vec[i]) != 0) {
      cout << "Line " << i+1 << " is incorrect" << endl;
      errors++;
    }
  }
  cout << "Comparison completed. " << errors << " errors." << endl;
  return 0;
}
