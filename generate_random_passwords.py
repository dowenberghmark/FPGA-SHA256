#!/usr/bin/env python3
import os
import sys
  
def write_random_lowercase(size): # generates .txt file random passwords 30 chars long where one passwords counts as 64bytes
  num_of_pass = (size * 1000000) / 64
  num_of_chars = num_of_pass * 31
  min_lowercase_ascii = ord(b'a') # used to specifiy minimum ascii value 
  interval_lowercase_ascii = 26 # used to covert ascii values to range 97..122 (a-z)
  rand_byte_array = bytearray(os.urandom(num_of_chars))
  counter = 0;

  print "Number of passwords generated:", num_of_pass

  for i, b in enumerate(rand_byte_array): # convert byte array values to range 97..122 and replaces every 31th char with '\n'
    if counter == 30:
      rand_byte_array[i] = '\n'
      counter = 0
    else:
      rand_byte_array[i] = min_lowercase_ascii + b % interval_lowercase_ascii # convert 0..255 to 97..122 (a-z)
      counter += 1

  with open("random_passwords.txt", "w") as text_file:
    text_file.write("%s" % rand_byte_array)

def main(argv):
  if len(argv) == 2:
    write_random_lowercase(int(argv[1]))
  else:
    print "Usage: genereate_random_passwords [megabytes of passwords to generate]"
  
if __name__ == "__main__":
  main(sys.argv)
  print "All done"


