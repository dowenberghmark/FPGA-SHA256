#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <chrono>
#include <bitset>
#include "sha256.hpp"

using namespace std;
typedef unsigned char BYTE;
/** 
 * Appends char one to input string.
 * Appends '1' to a string which is in binary format
 */
string append_one(string s){
	s.append("1");
	return s;
}

/** 
 * Appends k char zeros to s1.
 * Appends k (k = 448 - length(string)) '0' to s1
 * which is in a binary format.
 */
string append_k(string s){
	size_t k = 448 - s.length();
	s.append(k, '0');
	return s;
}

/** 
 * Appends 64-bits representing the size of s2 to s1.
 * Appends 64-bits representing (8 * length(s2)) in binary to s1
 * which is in a binary format.
 */
string append_64(string s1, string s2){
	int s2_size = (8 * s2.length());
	cout << s2_size << "\n";
	s1.append(bitset<64>(s2_size).to_string());
	return s1;
}




// BYTE = UNSIGNED CHAR
vector<BYTE> password_prepare(string filename){
//	ifstream file;
//  file.open(filename);

 // open the file:
    std::ifstream file(filename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(509, std::ios::end);
    memset(file,'0',509);
    fileSize = file.tellg();
    int length = file.tellg();
    cout << "length is: " << length << endl;
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<BYTE> vec;
    vec.reserve(512);
    

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<BYTE>(file),
               std::istream_iterator<BYTE>());

    for (auto && n : vec){
      cout << "TEST PRINTING VEC: " << bitset<8>(n) << endl;

    }


// PRINTA BITS



    return vec;

}




int main() {

	// PREPARE FOR HASHING
  auto start = chrono::steady_clock::now();
   vector<BYTE> prepared_passwords = password_prepare("password.txt");




   return 0;
}