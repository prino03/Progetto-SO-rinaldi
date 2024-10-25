CC     = gcc
CCOPTS = -std=gnu99 -Wall
AR     = ar

SRC     = pseudo_malloc.c buddyalloc_bitmap.c
HEADERS = pseudo_malloc.h buddyalloc_bitmap.h
OBJS    = pseudo_malloc.o buddyalloc_bitmap.o
LIB    = libpseudo.a
EXE    = main

.PHONY: clean all

all: $(LIB) $(EXE)

%.o: %.c
	$(CC) $(CCOPTS) -c -o $@ $<

libpseudo.a: $(OBJS)
	$(AR) -rcs $@ $(OBJS)

main: main.o $(LIB)
	$(CC) $(CCOPTS) -o $@ $^ -lm


clean:	
	rm -f *.o $(lib) $(exe)
