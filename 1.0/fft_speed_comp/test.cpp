#include<cmath>
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<complex>
#include<../include/kiss_fft.h>
#include<../include/kiss_fftr.h>
#define PI 3.1459265

using namespace std;


float* cosine(int n)
{
	int i = 0;

	float* cosgn = new float[n];
	float step = 2/n;

	for (i = 1; i < n+1; i++) {
		cosgn[i] = cos( 2.0 * PI * 60.0 * step*i );
	}

	return cosgn;
}


clock_t dft( complex<float>* x, int NFFT)
{
	clock_t start, end;
	start = clock();

	complex<float>* X = new complex<float>[NFFT];
	complex<float> twiddle;

	for(int f = 0; f < NFFT; f++)
	{
		for (int n = 0; n < NFFT; n++) {
			twiddle = polar(1.0, (-2*PI*n*f)/NFFT);	
			X[f] += x[n]*twiddle;
		}
	}

	end = clock();
	

	delete [] X; 

	return (end - start);
}



complex<float>* FFT( complex<float>* x, int NFFT )
{
	complex<float>* X = new complex<float>[NFFT];

	if ( NFFT == 1 )
	{
		X[0] = x[0];
		return X;
	}

	else
	{
		complex<float>* even = new complex<float>[NFFT/2];
		complex<float>* odd  = new complex<float>[NFFT/2];
		int n;

		for(n = 0; n < NFFT/2; n++)
		{
			even[n] = x[2*n];
			odd[n]  = x[2*n+1];
		}

		complex<float>* EVEN = FFT( even, NFFT/2 );
		complex<float>* ODD = FFT( odd, NFFT/2 );
		complex<float> twiddle;

		delete [] even;
		delete [] odd;

		for(n = 0; n < NFFT/2; n++)
		{
			twiddle= polar( 1.0, -(2*PI*n)/NFFT );
			ODD[n] = ODD[n] * twiddle;
			X[n]   = EVEN[n] + ODD[n];
			X[n+NFFT/2] = EVEN[n] - ODD[n];
		}

		delete [] EVEN;
		delete [] ODD;

		return X;
	}
}

clock_t fft(complex<float>* sig, int n)
{
	clock_t start, end;
	start = clock();

	complex<float>* X = FFT(sig, n);

	end = clock();


	delete [] X;

	return double(end - start);
}

clock_t kiss(float* sig, int NFFT)
{
	clock_t start, end;
	kiss_fftr_cfg	cfg = NULL;
	kiss_fft_scalar	rin[NFFT];
	kiss_fft_cpx	sout[NFFT];

	start = clock();
	
	cfg = kiss_fftr_alloc(NFFT, 0, NULL, NULL);
	for(int	i=0; i < NFFT; i++)
	{
		rin[i] = sig[i];
	}

	kiss_fftr(cfg, rin, sout);

	end = clock();

	return double(end-start);
}



int main(int argc, const char *argv[])
{

	int NFFT = atoi(argv[1]);
	float* sig = cosine(NFFT);
	complex<float>* x = new complex<float>[NFFT];

	for (int i = 0; i <NFFT; i++) {
		x[i].real() = sig[i];
	}

	cout << "Signal created" << endl;
	
	cout << "FFT:"<< endl;
	clock_t t2 = fft(x, NFFT);
	cout << "\t" << (double(t2) / double(CLOCKS_PER_SEC))*1000 << "\tms" << endl;


	cout << "DFT:"<< endl;
	clock_t t1 = dft(x, NFFT);
	cout << "\t" << (double(t1) / double(CLOCKS_PER_SEC))*1000 << "\tms" << endl;

	cout << "KISS: "<< endl;
	clock_t t3 = kiss(sig, NFFT);
	cout << "\t" << (double(t3) / double(CLOCKS_PER_SEC))*1000 << "\tms" << endl;


	delete [] x;
	delete [] sig;
	
	return 0;
}
