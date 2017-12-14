CC = g++
CFLAGS = -std=c++11 -g -Wall -Werror -O3

LDLIBS = -lcrypto

SRC = host_sha256.cpp
BIN = host_sha256

OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	rpm -qa | grep -q openssl-devel || sudo yum install --assumeyes openssl-devel
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

installOpenSSL:
	sudo yum install openssl-devel

clean:
	rm -f *.o $(BIN)
