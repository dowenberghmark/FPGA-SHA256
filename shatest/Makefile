CC = g++
CFLAGS = -Wall
OBJS = sha.o sha256.o sha_preprocess.o
TARGET = sha

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) sha.c sha256.c sha_preprocess.cpp print_bits.cpp
