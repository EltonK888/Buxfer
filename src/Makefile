CC = gcc
CFLAGS = -Wall -Werror -g

buxfer: buxfer.o lists.o lists.h
	$(CC) $(CFLAGS) -o buxfer buxfer.o lists.o

buxfer.o: buxfer.c lists.h
	$(CC) $(CFLAGS) -c buxfer.c

lists.o: lists.c lists.h
	$(CC) $(CFLAGS) -c lists.c

clean: 
	rm buxfer *.o
