CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

csim: main.o
	$(CC) -o csim main.o -g

main.o: main.c main.h
	$(CC) $(CFLAGS) -c main.c -g

clean:
	rm -f *.o hw3 *~
