CC = c++
CFLAGS = -std=c++11 -pedantic -Wall -Wextra 

%.o : %.cpp
	gcc $(CFLAGS) -c $<

csim: csim.o cfuncs.o
	$(CC) -o csim csim.o cfuncs.o -g
clean:
	rm -f *.o csim *~ *.zip

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp
