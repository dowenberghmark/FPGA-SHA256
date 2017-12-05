#!/bin/bash

#Generate $1 number of random string that are 20 characters long and outputs into 'rand_pass.txt'


if [ $# -ne 1 ]; then
    echo "Usage: ./gen_passwords.sh [num of passwords]"
else
	counter=$1
	until [ $counter -lt 1 ]
	do
		res=$( cat /dev/random | LC_CTYPE=C tr -dc "[:alpha:]" | head -c 30 )
		echo $res >> rand_pass.txt
		((counter--))
	done
	echo All done
fi
