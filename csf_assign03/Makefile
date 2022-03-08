CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

csim: csim_main.o
	$(CC) -o csim csim_main.o -g

main.o: csim_main.c
	$(CC) $(CFLAGS) -c csim_main.c -g

clean:
	rm -f *.o csim *~
