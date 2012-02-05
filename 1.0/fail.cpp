#include "RtAudio.h"
#include "include/kiss_fft.h"
#include "include/kiss_fftr.h"
#include <iostream>
#include <string>
#include <cstdlib>
//#include <cstring>
#include <complex>
#include <cmath>
#include <fstream>


using namespace std;

typedef float type;



struct note {
	static const float f[];
	static const string p[];
	static const int total;
} notes;

const float note::f[] = {
	61.735,
	65.406, 69.296, 73.416, 77.782, 82.407, 87.307, 92.499, 97.999, 103.83, 110.00, 116.54, 123.47,
	130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
	261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 416.30, 440.00, 466.16, 493.88
};

const string note::p[] = {
	"B1",
	"C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2",
	"C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
	"C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4"
};
const int note::total = 37;


string findNote(float fund)
{
	for (int i = 0; i < notes.total; i++) {
		if (abs(fund - notes.f[i] )< 5 ) {
			return notes.p[i];
		}
	}
	return "No note found"; 
}

struct data{
	type* buffer;		//location of data
	unsigned int fs;		//sample rate
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

int record(void *, void *inputBuffer, unsigned int nBufferFrames,
		double, RtAudioStreamStatus status, void *userData )
{
	data* d = (data*) userData;
	type *ins = (type *) inputBuffer;

	if ( status )
		std::cout << "Stream overflow detected!" << std::endl;

	unsigned int frames = nBufferFrames;		//number of new frames

	int 	i = 0;
	int 	NFFT = (int)d->totalFrames;
	int 	fs = (int) d->fs;
	kiss_fftr_cfg	cfg = NULL;
	kiss_fft_scalar	rin[NFFT];
	kiss_fft_cpx	sout[NFFT];
	float		freq[NFFT];
	float		fft[NFFT];
	float fres = (float)( fs / NFFT );
	float temp;


	//If # new frames is more than it can hold,
	//Only copy in as much as buffer size will hold

	if( d->frameCounter + frames > d->totalFrames ) {
		frames = d->totalFrames - d->frameCounter;
		d->bufferBytes = frames * d->channels * sizeof( type );
	}


		//Copy the new data at the starting position
	unsigned long start = d->frameCounter * d->channels;

	memcpy( d->buffer + start, inputBuffer, d->bufferBytes);

	//Increment frame counter
	d->frameCounter += frames;

	//Check boundary once more
	if (d->frameCounter >= d->totalFrames ) {
	d->frameCounter = 0;
	}

	//while(d->frameCounter < d->totalFrames){

		//if( d->frameCounter + frames > d->totalFrames ) {
			//frames = d->totalFrames - d->frameCounter;
			//d->bufferBytes = frames * d->channels * sizeof( type );
		//}


		////for (i = 0; i <(int)d->totalFrames; i++, d->frameCounter++) {
			////d->buffer[i] = *ins;
		////}
		////
		//memcpy ( d->buffer, ins , d->bufferBytes);

		
		//d->frameCounter += frames;

		//NFFT = (int)d->totalFrames;
		//cfg = kiss_fftr_alloc(NFFT, 0, NULL, NULL);

		//if ((int) d->frameCounter == NFFT) {
			memcpy( rin, d->buffer, NFFT*sizeof(float));


			//for(i=0; i < NFFT; i++){
			//rin[i] = (float)d->buffer[i]; 
			//}

			kiss_fftr(cfg, rin, sout);

			//Analyze the frequency
			for (i = 0; i < NFFT; i++) {
				freq[i] = ((float)i)*fres;
				temp = (float) sout[i].r;
				fft[i] = abs(temp);
			}


			float		fund = 0;
			int		fundi = 0;
			for (i=0; i< NFFT; i++) {
				if( sout[i].r > fund )
				{
					fund = fft[i];
					fundi = i;
				}
			}

			system("clear");
			//cout << freq[fundi] << "\t" << fund << endl; 

			string NOTE = findNote( freq[fundi] );

			cout << "Frequency:\t" << freq[fundi] << endl;
			cout << "Note:\t\t" << NOTE << endl;
		//}

		//else
		//{
			//free(cfg);
			//return 2;
		//}
	//}

	//int	fs = (int)d->fs;
	//int	NFFT = (int)d->totalFrames;

	//kiss_fftr_cfg	cfg = NULL;
	//kiss_fft_scalar	rin[NFFT];
	//kiss_fft_cpx	sout[NFFT];
	//float		freq[NFFT];
	//float		fft[NFFT];
	//float fres = (float) fs / NFFT;
	//float temp;

	//cfg = kiss_fftr_alloc(NFFT, 0, NULL, NULL);

	//for(i=0; i < NFFT; i++){
	//rin[i] = (float)d->buffer[i]; 
	//}

	//kiss_fftr(cfg, rin, sout);


	//Analyze the frequency
	//for (i = 0; i < NFFT; i++) {
	//freq[i] = ((float)i)*fres;
	//temp = (float) sout[i].r;
	//fft[i] = abs(temp);
	//}


	//float		fund = 0;
	//int		fundi = 0;
	//for (i=0; i< NFFT; i++) {
	//if( sout[i].r > fund )
	//{
	//fund = fft[i];
	//fundi = i;
	//}

	//}
	//system("clear");
	////cout << freq[fundi] << "\t" << fund << endl; 

	//string NOTE = findNote( freq[fundi] );

	//cout << "Frequency:\t" << freq[fundi] << endl;
	//cout << "Note:\t\t" << NOTE << endl;


	free(cfg);

	return 0;
}

int main()
{


	RtAudio adc;
	if ( adc.getDeviceCount() < 1 ) {
		std::cout << "\nNo audio devices found!\n";
		exit( 0 );
	}
	adc.showWarnings( true );
	float t = 2.0;
	//int i = 0;

	//Initialize ADC parameters
	RtAudio::StreamParameters parameters;
	parameters.deviceId = adc.getDefaultInputDevice();
	parameters.nChannels = 1;
	parameters.firstChannel = 0;
	unsigned int fs = 44100;
	unsigned int bufferFrames = 256;
	unsigned long space;
	data datam;

	//RtAudio::StreamParameters oParams;
	//oParams.deviceId = adc.getDefaultOutputDevice();
	//oParams.nChannels = 1;
	//oParams.firstChannel = 0;


	//Initialize storage space
	datam.fs = fs;
	datam.channels = parameters.nChannels;
	datam.bufferBytes = bufferFrames * datam.channels * sizeof( type );
	datam.totalFrames = (unsigned long) t*fs;
	datam.frameCounter = 0;
	space = datam.totalFrames * datam.channels * sizeof(type);
	datam.buffer = (type*)malloc(space);

	//Open the stream and start recording
	try {
		adc.openStream( NULL, &parameters, RTAUDIO_FLOAT32,
				fs, &bufferFrames, &record, (void *)&datam );
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
		clean(adc, datam);
	}

	//Close stream and free memory
	if ( adc.isStreamOpen() ) adc.closeStream();
	if (datam.buffer) free(datam.buffer);



	return 0;
}
