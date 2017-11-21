#!/bin/bash

#Generate $1 megabytes of random strings that are 20 characters long and outputs into 'rand_pass.txt'

if [ $# -ne 1 ]; then
    echo "Usage: ./gen_passwords.sh [megabytes of passwords]"
else
    > random_passwords.txt
    pass_size=$(($1*1000000))
    counter=$(($pass_size/64))
    echo "Generating $counter passwords"
    until [ $counter -lt 1 ]
    do
	res=$( cat /dev/urandom | LC_CTYPE=C tr -dc "[:alpha:]" | head -c 30 )
	echo $res >> random_passwords.txt
	((counter--))
    done
    echo All done
fi
