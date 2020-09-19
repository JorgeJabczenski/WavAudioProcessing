# GRR20190372 Jorge Lucas Vicilli Jabczenski

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

progs = wavinfo wavvol wavnorm wavrev wavecho wavwide wavcat wavmix
objs  = wav.o funwav.o

all: $(progs)

wavinfo: $(objs) wavinfo.o
wavvol : $(objs) wavvol.o
wavnorm: $(objs) wavnorm.o
wavrev : $(objs) wavrev.o
wavecho: $(objs) wavecho.o
wavwide: $(objs) wavwide.o
wavcat : $(objs) wavcat.o
wavmix : $(objs) wavmix.o

wav.o: wav.h wav.c 
funwav.o: wav.o funwav.h funwav.c 

clean: 
	rm *.o

purge: clean
	rm $(progs)