CC=`which gcc`
LD=$(CC)
RM=`which rm` -f


SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

CFLAGS=-O2 -Wall -g -ansi -pedantic-errors -I./include/ 
LFLAGS=#-lefence

BIN=daemon

all : $(BIN)


$(BIN) : $(OBJ)
	$(LD) $(LFLAGS) $^ -o $@ 

.c.o : 
	$(CC) $(CFLAGS) $< -c

clean : 
	$(RM) $(BIN) $(OBJ) *~ semantic.cache

exe : 
	./$(BIN)
