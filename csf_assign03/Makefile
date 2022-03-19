CC = c++
CFLAGS = -std=c++11 -pedantic -Wall -Wextra

%.o : %.cpp
	gcc $(CFLAGS) -c $<

csim: csim.o cfuncs.o
	$(CC) -o csim csim.o cfuncs.o -g
<<<<<<< HEAD

=======
	
>>>>>>> 915ddd9ba1c2de77ad92e76818d8d3aa1836a9c6
clean:
	rm -f *.o csim *~

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
<<<<<<< HEAD
	zip -9r $@ Makefile README.txt *.h *.c *.S
=======
	zip -9r $@ Makefile README.txt *.h *.cpp
>>>>>>> 915ddd9ba1c2de77ad92e76818d8d3aa1836a9c6
