CC = g++
CXX = g++
CPPFLAGS = -Wall -O3 -std=c++11
LDFLAGS = -O3
CFLAGS =

all: main

main: main.o RedeNeural.o

main.o: main.cpp

RedeNeural.o: RedeNeural.h RedeNeural.cpp

##### SPECIAL TARGETS ######

.PHONY: clean realclean 

clean:
	-rm -f *.o *~ 

realclean: clean
	-rm -f main


