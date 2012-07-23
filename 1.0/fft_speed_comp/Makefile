PROGRAMS = test
SRC_PATH = .
INCLUDE = ../include/
OBJECT_PATH = ../include/
vpath %.o $(OBJECT_PATH)

OBJECTS = kiss_fft.o kiss_fftr.o

CC 	= g++
CFLAGS 	= -g -O2 -Wall -I$(INCLUDE)

all : $(PROGRAMS)

kiss_fft.o: ../include/kiss_fft.c ../include/_kiss_fft_guts.h
	gcc -c ../include/kiss_fft.c -o ../include/_kiss_fft_guts.h

kiss_fftr.o: ../include/kiss_fftr.c ../include/_kiss_fft_guts.h
	gcc -c ../include/kiss_fftr.c -o ../include/_kiss_fft_guts.h

test:	test.cpp $(OBJECTPATH)kiss_fft.o $(OBJECTPATH)kiss_fftr.o
	$(CC) $(CFLAGS) -o test test.cpp ../include/kiss_fft.o ../include/kiss_fftr.o
