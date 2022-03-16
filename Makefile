SRC = $(wildcard ./src/*.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -ggdb -g3 
LDLIBS = -lX11
INC = $(wildcard ./src/*.h)
CC = gcc
TGT = $(shell basename `pwd`)

$(TGT): 
	$(CC) $(CFLAGS) -o $(TGT) $(SRC) $(LDLIBS)
.PHONY: clean

clean:
	rm -f $(TGT) $(OBJ)

install: $(TGT)
	sudo mv $(TGT) /usr/local/bin
