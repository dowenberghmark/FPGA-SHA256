#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <chrono>
using namespace std;

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

std::string ascii_to_bits(string s){
	string binary_string;
	int ascii;
  		for(int i=0; i<s.length(); i++){
  			ascii = s[i];
  			char* binary_reverse = new char [9];
  			char* binary = new char[9];
  			int j = 0; //array counter
  			while(ascii !=1){
  				if(ascii % 2 == 0){
  					binary_reverse[j] = '0';
  				}
  				else if(ascii % 2 == 1){
  					binary_reverse[j] = '1';
  				}
  				ascii /=2;
  				j++;
  			}

  			if(ascii == 1){
  				binary_reverse[j] = '1';
  				j++;
  			}
  			if(j < 8){
  				for(;j<8;j++){
  					binary_reverse[j] = '0';
  				}
  			}
  			for(int k=0; k<8;k++){
  				binary[k] = binary_reverse[7 - k];
  			}

  			binary_string += binary;

  		}
  		return binary_string;
}


std::vector<std::string> password_prepare(string filename){
	

	ifstream file;
   file.open(filename);
  	string element;
  	vector<string> vector_array;
  	while(!file.eof()){
  		file >> element;
  		
  		/*HÄR SKA KODEN IN*/
  		string password = ascii_to_bits(element);
  		password = append_one(password);
  		password = append_k(password);
  		password = append_64(password,element);
		vector_array.push_back(password);  //skriver den i en vectory array
	
  	}
   file.close();
   return vector_array;
}




int main() {
  auto start = chrono::steady_clock::now();
   vector<string> prepared_passwords = password_prepare("password.txt");
   for (string n : prepared_passwords){
   	//std::cout << n << '\n';
   	//std::cout << "size is: " << n.size() << "\n";
   }


   auto end_pre_hashing = chrono::steady_clock::now();
   auto pre_hashing_time = end_pre_hashing - start;

   auto sending_chunk = chrono::steady_clock::now();
   //SEND CHUNK
   auto recieving_chunk = chrono::steady_clock::now();
   auto chunk_transport_time = recieving_chunk - sending_chunk; 

   // When all passwords have been hashed
   auto end = chrono::steady_clock::now();

   auto diff = end - start;

   cout << "===========================================" << endl;
   cout << "Pre-hashing passwords... " << endl;
   cout << "Pre-hashing time: ";
   cout << chrono::duration <double, milli> (pre_hashing_time).count() << " ms" << endl;
   cout << "===========================================" << endl;
   cout << "\n";
   cout << "\n";
   cout << "===========================================" << endl;
   cout << "Sending chunk.... " << endl;

   cout << "Chunk transport time: ";
   cout << chrono::duration <double, milli> (chunk_transport_time).count() << " ms" << endl;
   cout << "===========================================" << endl;
   cout << "\n";
   cout << "\n";
   cout << "===========================================" << endl;
   cout << "Total time: ";
   cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
   cout << "===========================================" << endl;



   return 0;
}