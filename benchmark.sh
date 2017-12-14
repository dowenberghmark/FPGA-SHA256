#!/usr/bin/env bash
if [ $# -ne 3 ] ;
then
    echo "./benchmark.sh <path_to_output> <path_to_password_file> <host/hw>"
    exit
fi
F=$1 #  Name of path/output file
PASS=$2 # Path to password file
PROGRAM="./sha256 -b" # Not in use at the moment
SIZE=(0.1 1 10 100 1000 10000 100000 1000000 10000000 100000000)
N=10
CPU='host'

if [ -f "$F" ]
then
    rm $F
fi

if [ $3 = $CPU ]
then
    PROGRAM="./host/host_sha256"
fi

echo "Starting benchmarks for $((N)) iterations."
# Start 
for  s in ${SIZE[*]}; do 		
    for ((i=1;i<$((N+1));i++));	do 			
	echo "Iteration: $i, Size: $s"
	eval $PROGRAM  -s $s -f $PASS -o $F
    done
done
