CC=`which gcc`
LD=$(CC)
RM=`which rm` -f


SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

CFLAGS=-Wall -g -I./include/ -ansi -pedantic-errors
LFLAGS=-pthread

BIN=parallel

all : $(BIN) cas.s


$(BIN) : $(OBJ) cas.o
	$(LD) $(LFLAGS) $^ -o $@ 

.c.o : 
	$(CC) $(CFLAGS) $< -c

cas.s:
	as $(CFLAGS) cas.s -o cas.o

clean : 
	$(RM) $(BIN) $(OBJ) cas.o *~ semantic.cache

exe : 
	./$(BIN)
