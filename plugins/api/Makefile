CC=`which gcc`
RM=`which rm` -f

TARGET=libmat.so
SRC=mat.c

CFLAGS=-Wall -O2 -fPIC -shared

install : $(TARGET)

all : $(TARGET)

$(TARGET) : $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ 

clean : 
	$(RM) $(TARGET) *~


