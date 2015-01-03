PROGRAMS = main 
RM = /bin/rm
SRC_PATH = . 
RTAUDIO = rtaudio-4.1.1
KISSDIR = kiss_fft130

LIBRTAUDIO = $(RTAUDIO)/librtaudio.a

OBJECTS	=	$(LIBRTAUDIO) $(KISSDIR)/kiss_fft.o $(KISSDIR)/kiss_fftr.o

CC       = g++
DEFS     =   -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__
CFLAGS   = -g -O2 -Wall -framework CoreAudio -framework CoreServices
CFLAGS  += -I$(KISSDIR) -I$(KISSDIR)/tools -I$(RTAUDIO)
#LIBRARY  = -lpthread -lasound -lm
LIBRARY  = -lpthread -lm

all : $(PROGRAMS)

$(KISSDIR)/kiss_fft.o: $(KISSDIR)/kiss_fft.c | $(KISSDIR)
	cd $(KISSDIR); gcc -c kiss_fft.c -o kiss_fft.o

$(KISSDIR)/kiss_fftr.o: $(KISSDIR)/tools/kiss_fftr.c | $(KISSDIR)
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

$(KISSDIR): | $(KISSDIR).zip
	unzip $(KISSDIR).zip

clean : | $(RTAUDIO)/Makefile
	$(RM)	-f *.txt
	make --directory=$(RTAUDIO) clean
	make --directory=$(KISSDIR) clean
	$(RM)	-f $(OBJECTS)

strip : 
	strip $(PROGRAMS)
