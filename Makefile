# GRR20190372 Jorge Lucas Vicilli Jabczenski

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

progs = wavinfo wavvol wavnorm wavrev wavecho wavwide wavcat wavmix
objs  = estruturas.h acesso.o funwav.o

all: $(progs)

wavinfo: $(objs) wavinfo.o
wavvol : $(objs) wavvol.o
wavnorm: $(objs) wavnorm.o
wavrev : $(objs) wavrev.o
wavecho: $(objs) wavecho.o
wavwide: $(objs) wavwide.o
wavcat : $(objs) wavcat.o
wavmix : $(objs) wavmix.o

acesso.o: estruturas.h acesso.h acesso.c 
funwav.o: estruturas.h funwav.h funwav.c 

clean: 
	rm *.o

purge: clean
	rm $(progs)