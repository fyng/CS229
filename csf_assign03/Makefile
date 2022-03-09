CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

csim: csim_main.o
	$(CC) -o csim csim_main.o -g

main.o: csim_main.c
	$(CC) $(CFLAGS) -c csim_main.c -g

clean:
	rm -f *.o csim *~

# Use this target to create a zipfile that you can submit to Gradescope
.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.c *.S