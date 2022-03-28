CC = g++
CFLAGS = -std=c++11 -pedantic -Wall -Wextra 

%.o : %.cpp
	g++ $(CFLAGS) -c -g $<

csim: csim.o cfuncs.o
	$(CC) -o csim csim.o cfuncs.o 
clean:
	rm -f *.o csim *~ *.zip

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *.h *.cpp
