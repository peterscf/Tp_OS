
CC=`which gcc`
RM=`which rm` -f

CFLAGS=-g -Wall -ansi -I./include -D_XOPEN_SOURCE

all: server odd random smart

server: server.o mechanism.o
	$(CC) -o $@  $^ $(CFLAGS)

odd: player.c
	$(CC) -o $@ $^ $(CFLAGS) -D_ODD

random: player.c
	$(CC) -o $@ $^ $(CFLAGS) -D_RANDOM

smart: player.c
	$(CC) -o $@ $^ $(CFLAGS) -D_SMART

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@ -c

clean:
	$(RM) *.o *~ include/*~ server odd random smart
