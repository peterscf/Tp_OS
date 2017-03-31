CC=`which gcc`
LD=`which gcc`
RM=`which rm` -f

CFLAGS=-Wall -O2 -fPIC

BIN=test
BIN_SRC=test.c
BIN_OBJ=$(BIN_SRC:.c=.o)

BIN_LFLAGS=-L. -lfence

LIB=libfence.so
LIB_SRC=alloc.c
LIB_OBJ=$(LIB_SRC:.c=.o)

LIB_LFLAGS=-shared

all : $(LIB) $(BIN)

$(BIN) : $(BIN_OBJ)
	$(LD) $(BIN_LFLAGS) $^ -o $@ 

$(LIB) : $(LIB_OBJ)
	$(LD) $(LIB_LFLAGS) $^ -o $@ 

.c.o : 
	$(CC) $(CFLAGS) $< -c

clean : 
	$(RM) $(BIN_OBJ) $(LIB_OBJ) $(BIN) $(LIB) *~

exe : 
	./$(BIN)
