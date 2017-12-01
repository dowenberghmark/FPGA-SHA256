CC = g++
CFLAGS = -Wall -std=c++11
OBJS = sha.o sha256.o sha_preprocess.o
TARGET = sha

$(TARGET): sha256.c sha_preprocess.cpp sha.c
	$(CC) $(CFLAGS) -o $(TARGET) sha.c sha256.c sha_preprocess.cpp print_bits.cpp
