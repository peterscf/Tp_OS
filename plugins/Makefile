
BIN=imlab

CC=`which gcc`
LD=`which gcc`
RM=`which rm` -f

CFLAGS=-Wall -O2 -I include/
LFLAGS=-ldl api/libmat.so

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

PLUG_DIR=plugins

all : $(BIN)

$(BIN) : $(OBJ)
	$(LD) $^ $(LFLAGS) -o $@

.c.o : 
	$(CC) $(CFLAGS) $< -c

clean : 
	$(RM) $(OBJ) $(BIN) *~

exe : 
	./$(BIN)

install : 
	cd api && make -s install && cd ..
	cd plugins && make -s install && cd .. 

deep-clean : 
	make -s clean 
	cd api && make -s clean && cd ..
	cd plugins && make -s clean && cd ..



