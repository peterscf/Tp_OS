
PLUGINS_SRC=$(wildcard *.c)

PLUGINS=$(PLUGINS_SRC:.c=.so)

CFLAGS=-Wall -O2 -fPIC -shared -I ../api
LFLAGS=api/libmat.so -lm

# Rajouter le suffixe .so a la 
# liste des suffixes connus.
# On aurait pu suffixer nos plugins
# .sfx ou n'importe quoi d'autre. 
# Mais .so est plus comprehensible. 
.SUFFIXES: .so

all : $(PLUGINS)

.c.so : 
	$(CC) $(CFLAGS) $(LFLAGS) $< -o $@

clean : 
	$(RM) $(PLUGINS) *~

install :
	rm -f api && ln -s ../api api 
	make all
