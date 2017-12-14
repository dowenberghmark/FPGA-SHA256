CC = g++
CFLAGS = -std=c++11 -g -Wall -Werror -O3

LDLIBS = -lcrypto

SRC = host_sha256.cpp host_benchmark.cpp
BIN = host_sha256

OBJ = $(SRC:.c=.o)

all: installOpenSSL $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

installOpenSSL:
	rpm -qa | grep -q openssl-devel || sudo yum install --assumeyes openssl-devel

clean:
	rm -f *.o $(BIN)
