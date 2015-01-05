PROGRAMS = test
SRC_PATH = .
KISSDIR = ../kissfft-code

OBJECTS = $(KISSDIR)/kiss_fft.o $(KISSDIR)/kiss_fftr.o

CC 	= g++
CFLAGS 	= -g -O2 -Wall -I$(KISSDIR) -I$(KISSDIR)/tools

all : $(PROGRAMS)

$(KISSDIR)/kiss_fft.o: $(KISSDIR)/kiss_fft.c
	cd $(KISSDIR); gcc -c kiss_fft.c -o kiss_fft.o

$(KISSDIR)/kiss_fftr.o: $(KISSDIR)/tools/kiss_fftr.c
	cd $(KISSDIR); gcc -c tools/kiss_fftr.c -I. -o kiss_fftr.o

test:	test.cpp $(OBJECTS)
	$(CC) $(CFLAGS) -o test test.cpp $(OBJECTS)
