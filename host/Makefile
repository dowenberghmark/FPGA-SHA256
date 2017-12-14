CC = g++
CFLAGS = -std=c++11 -g -Wall -Werror -O3

LDLIBS = -lcrypto

SRC = host_sha256.cpp
BIN = host_sha256

OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o $(BIN)
