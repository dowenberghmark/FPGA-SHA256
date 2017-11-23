#!/usr/bin/env python3
import os
import sys
  
def write_random_lowercase(size):
  num_of_pass = (size * 1000000) / 64
  n = num_of_pass * 31
  min_lc = ord(b'a')
  len_lc = 26
  ba = bytearray(os.urandom(n))
  counter = 0;
  
  print "Number of passwords generated:", num_of_pass

  for i, b in enumerate(ba):
    if counter == 30:
      ba[i] = '\n'
      counter = 0
    else:
      ba[i] = min_lc + b % len_lc # convert 0..255 to 97..122
      counter += 1

  del ba[-1]
  with open("Output.txt", "w") as text_file:
    text_file.write("%s" % ba)

def main(argv):
  if len(argv) == 2:
    write_random_lowercase(int(argv[1]))
  else:
    print "Usage: genereate_random_passwords [megabytes of passwords to generate]"
  
if __name__ == "__main__":
  main(sys.argv)
  print "All done"


