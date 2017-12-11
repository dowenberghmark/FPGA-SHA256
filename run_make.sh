#!/usr/bin/env bash
flag=sw_emu

if [ $# -ne 0 ]
then
    if [ $1 == 'hw' ]
    then
	flag=hw_emu
    fi
fi

echo
echo Running with flag $flag
echo
make clean
make check TARGETS=$flag DEVICES=$AWS_PLATFORM
