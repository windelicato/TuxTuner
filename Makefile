PROGRAMS = main 
RM = /bin/rm
SRC_PATH = . 
RTAUDIO = rtaudio-4.1.1
KISSDIR = kissfft-code

LIBRTAUDIO = $(RTAUDIO)/librtaudio.a

OBJECTS = $(LIBRTAUDIO) $(KISSDIR)/kiss_fft.o $(KISSDIR)/kiss_fftr.o

CC       = g++
DEFS     = -DHAVE_GETTIMEOFDAY
CFLAGS   = -g -O2 -Wall
LIBRARY  = -lpthread -lm

ifeq ($(OS),Windows_NT)
    DEFS += -DWIN32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        CFLAGS += -framework CoreAudio -framework CoreServices
    else
        LIBRARY += -lasound
        DEFS    += -D__LINUX_ALSA__
    endif
endif

CFLAGS  += -I$(KISSDIR) -I$(KISSDIR)/tools -I$(RTAUDIO)

all : $(PROGRAMS)

$(KISSDIR)/kiss_fft.c: | $(KISSDIR)
$(KISSDIR)/tools/kiss_fftr.c: | $(KISSDIR)

$(KISSDIR)/kiss_fft.o: $(KISSDIR)/kiss_fft.c
	cd $(KISSDIR); gcc -c kiss_fft.c -o kiss_fft.o

$(KISSDIR)/kiss_fftr.o: $(KISSDIR)/tools/kiss_fftr.c
	cd $(KISSDIR); gcc -c tools/kiss_fftr.c -I. -o kiss_fftr.o

main : main.cpp $(OBJECTS)
	$(CC) $(CFLAGS) $(DEFS) -o main main.cpp $(OBJECTS) $(LIBRARY)
	
$(RTAUDIO).tar.gz:
	curl http://www.music.mcgill.ca/~gary/rtaudio/release/$(RTAUDIO).tar.gz --output $(RTAUDIO).tar.gz

$(RTAUDIO): | $(RTAUDIO).tar.gz
	tar xfvz $(RTAUDIO).tar.gz

$(RTAUDIO)/Makefile: | $(RTAUDIO)
	cd $(RTAUDIO); ./configure

$(LIBRTAUDIO): $(RTAUDIO)/Makefile
	make --directory=$(RTAUDIO)

$(KISSDIR):
	hg clone http://hg.code.sf.net/p/kissfft/code kissfft-code

clean : | $(RTAUDIO)/Makefile
	$(RM) -f *.txt
	$(RM) -f $(PROGRAMS)
	$(RM) -rf main.dSYM
	make --directory=$(RTAUDIO) clean
	make --directory=$(KISSDIR) clean
	$(RM) -f $(OBJECTS)

strip : 
	strip $(PROGRAMS)
