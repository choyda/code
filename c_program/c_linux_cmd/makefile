#!/bin/make

CC = gcc
CFLAGS = -g -Wall

SOURCE = $(wildcard *.c)
LINK = -lpcre -lm
PROGS = $(patsubst %.c,%,$(SOURCE))



all:	${PROGS}

$(PROGS):	clean	$(patsubst %,%.c,$@)
	$(CC) $(CFLAGS) -o $@ $(patsubst %,%.c,$@) $(LINK)

rebuild: clean	$(PROGS)
	

clean:
	rm -fr *.o *.pre ./lib/*.pre $(PROGS)
