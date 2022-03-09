CC = c++
CFLAGS = -std=c++11 -pedantic -Wall -Wextra

main: main.o
	$(CC) -o main main.o -g

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -g

clean:
	rm -f *.o csim *~

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.c *.S