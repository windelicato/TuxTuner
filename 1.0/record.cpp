#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <complex>


using namespace std;

typedef signed short BUFF;

struct data{
	signed short* buffer;		//location of data
	unsigned long bufferBytes;	//#of bytes of data
	unsigned long totalFrames;	//number of frames per sample (t * fs)
 	unsigned long frameCounter;	//number of current frames used
 	unsigned int channels;		//number of channels to record
 };


void clean(RtAudio adc, data d)
{
	if ( adc.isStreamOpen() ) adc.closeStream();
	if (d.buffer) free(d.buffer);
	exit(0);
}


int record( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		double streamTime, RtAudioStreamStatus status, void *userData )
{
 	data* d = (data*) userData;

	if ( status )
		std::cout << "Stream overflow detected!" << std::endl;
	
	unsigned int frames = nBufferFrames;		//number of new frames

	//If # new frames is more than it can hold,
	//Only copy in as much as buffer size will hold
	if( d->frameCounter + frames > d->totalFrames ) {
		frames = d->totalFrames - d->frameCounter;
		d->bufferBytes = frames * d->channels * sizeof( BUFF );
	}

	//Copy the new data at the starting position
	unsigned long start = d->frameCounter * d->channels;
	memcpy( d->buffer+start, inputBuffer, d->bufferBytes);

	//Increment frame counter
	d->frameCounter += frames;

	//Check boundary once more
	if (d->frameCounter >= d->totalFrames ) return 2;

	return 0;
}

int main()
{
	RtAudio adc;
	if ( adc.getDeviceCount() < 1 ) {
		std::cout << "\nNo audio devices found!\n";
		exit( 0 );
	}
	double t = 2.0;
	//Initialize ADC parameters
	RtAudio::StreamParameters parameters;
	parameters.deviceId = adc.getDefaultInputDevice();
	parameters.nChannels = 1;
	parameters.firstChannel = 0;
	unsigned int fs = 44100;
	unsigned int bufferFrames = 256;
	unsigned long space;
	data d;
	
	//Initialize storage space
	d.buffer = 0;
	d.channels = parameters.nChannels;
	d.bufferBytes = bufferFrames * d.channels * sizeof( BUFF );
	d.totalFrames = (unsigned long) t*fs;
	d.frameCounter = 0;
	space = d.totalFrames * d.channels * sizeof(BUFF);
	d.buffer = (BUFF*)malloc(space);

	//Open the stream and start recording
	try {
		adc.openStream( NULL, &parameters, RTAUDIO_SINT16,
				fs, &bufferFrames, &record, (void *)&d );
		adc.startStream();
	}
	catch ( RtError& e ) {
		e.printMessage();
		exit( 0 );
	}
	char input;
	std::cout << "\nRecording ... press <enter> to quit.\n";
	std::cin.get( input );

	

	// Stop the stream
	try {
		
		adc.stopStream();
	}
	catch (RtError& e) {
		e.printMessage();
		clean(adc, d);
	}
	
	



	
	//Close stream and free memory
	if ( adc.isStreamOpen() ) adc.closeStream();
	if (d.buffer) free(d.buffer);



	return 0;
}
