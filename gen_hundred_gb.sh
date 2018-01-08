#!/usr/bin/env bash

# generate 1 gb with python
python host/generate_random_passwords.py 1000

# append 1 gb passwords to hundredgigs.txt 100 times to create 100 gb passwords.
# 'du -h' should show 46 GB on hundredgigs.txt
for i in `seq 1 100`; do
    cat random_passwords.txt
done > hundredgigs.txt
