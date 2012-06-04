
CC = g++
CFLAGS = -Wall
VFLAGS = -Wall -g

all: iterator
	$(CC) $(CFLAGS) main.cpp -o utrp
debug:
	$(CC) $(VFLAGS) main.cpp -o utrp
iterator:	iterator.cpp
	$(CC) $(CFLAGS) iterator.cpp -o iterator
folders:
	mkdir salidas/
	mkdir reportes/
	mkdir entradas/
clean:
	rm iterator
	rm utrp
