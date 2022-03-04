SRC = $(wildcard ./src/*.c)
INC = $(wildcard ./src/*.h)
OBJ = $(SRC:.c=.o)
CFLAGS = -ggdb -g3 
LDLIBS = -lX11
CC = gcc
TGT = $(shell basename `pwd`)

$(TGT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS) && rm src/*.o
.PHONY: clean

clean:
	rm -f $(wildcard ./src/*.o) $(TGT) 

install: $(TGT)
	sudo mv $(TGT) /usr/local/bin
