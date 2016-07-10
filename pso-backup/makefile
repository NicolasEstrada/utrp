
CC = g++
CFLAGS = -Wall
VFLAGS = -Wall -g

all:
	$(CC) $(CFLAGS) main.cpp -o utrp
	$(CC) $(CFLAGS) iterator.cpp -o iter
debug:
	$(CC) $(VFLAGS) main.cpp -o utrp
	$(CC) $(VFLAGS) iterator.cpp -o iter
folders:
	mkdir salidas/
	mkdir reportes/
	mkdir entradas/
clean:
	rm iter
	rm utrp
