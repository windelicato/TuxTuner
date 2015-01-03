PROGRAMS = main 
RM = /bin/rm
SRC_PATH = . 
INCLUDE = include/
OBJECT_PATH = include/
vpath %.o $(OBJECT_PATH)
RTAUDIO = rtaudio-4.1.1

LIBRTAUDIO = $(RTAUDIO)/librtaudio.a

OBJECTS	=	$(LIBRTAUDIO) kiss_fft.o kiss_fftr.o

CC       = g++
DEFS     =   -DHAVE_GETTIMEOFDAY -D__LINUX_ALSA__
CFLAGS   = -g -O2 -Wall
CFLAGS  += -I$(INCLUDE) -Iinclude/
LIBRARY  = -lpthread -lasound -lm 

#%.o : $(SRC_PATH)/%.cpp
	##$(CC) $(CFLAGS) $(DEFS) -c $(<) -o $(OBJECT_PATH)/$@

#%.o : /include/%.cpp
	##$(CC) $(CFLAGS) $(DEFS) -c $(<) -o $(OBJECT_PATH)/$@
kiss_fft.o: include/kiss_fft.c include/_kiss_fft_guts.h
	gcc -c include/kiss_fftr.c -o include/kiss_fft.o
kiss_fftr.o: include/kiss_fft.c include/_kiss_fft_guts.h
	gcc -c include/kiss_fft.c -o include/kiss_fftr.o

all : $(PROGRAMS)

main : main.cpp $(OBJECTS)
	$(CC) $(CFLAGS) $(DEFS) -o main main.cpp $(OBJECT_PATH)*.o $(LIBRARY)
	
$(RTAUDIO).tar.gz:
	curl http://www.music.mcgill.ca/~gary/rtaudio/release/$(RTAUDIO).tar.gz --output $(RTAUDIO).tar.gz

$(RTAUDIO): | $(RTAUDIO).tar.gz
	tar xfvz $(RTAUDIO).tar.gz

$(RTAUDIO)/Makefile: | $(RTAUDIO)
	cd $(RTAUDIO); ./configure

$(LIBRTAUDIO): $(RTAUDIO)/Makefile
	make --directory=$(RTAUDIO)

clean : | $(RTAUDIO)/Makefile
	$(RM)	-f *.txt
	make --directory=$(RTAUDIO) clean
	


strip : 
	strip $(PROGRAMS)
