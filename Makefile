# GRR20190372 Jorge Lucas Vicilli Jabczenski

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

progs = wavinfo wavvol wavnorm wavrev wavecho wavwide wavcat wavmix
objs  = acesso.o funwav.o wavinfo.o wavvol.o wavnorm.o wavrev.o wavecho.o wavwide.o wavcat.o wavmix.o

all: $(progs)

wavinfo: acesso.o funwav.o wavinfo.o
wavvol : acesso.o funwav.o wavvol.o
wavnorm: acesso.o funwav.o wavnorm.o
wavrev : acesso.o funwav.o wavrev.o
wavecho: acesso.o funwav.o wavecho.o
wavwide: acesso.o funwav.o wavwide.o
wavcat : acesso.o funwav.o wavcat.o
wavmix : acesso.o funwav.o wavmix.o

acesso.o: estruturas.h acesso.h acesso.c 
funwav.o: estruturas.h funwav.h funwav.c 

wavinfo.o: estruturas.h wavinfo.c
wavvol.o : estruturas.h wavvol.c
wavnorm.o: estruturas.h wavnorm.c
wavrev.o : estruturas.h wavrev.c
wavecho.o: estruturas.h wavecho.c
wavwide.o: estruturas.h wavwide.c
wavcat.o : estruturas.h wavcat.c
wavmix.o : estruturas.h wavmix.c


clean: 
	rm $(objs)

purge: clean
	rm $(progs)