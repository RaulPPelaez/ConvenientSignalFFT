/* Raul P. Pelaez 2017. Fast Fourier Transform FFTW */


#include<iostream>
#include<vector>
#include<cmath>

#include<fftw3.h>
#include"vector_algebra.h"
#include"superRead.h"



template<class floatType>
void FFTCPUMode( int numberElements, double Fs){
  
  FILE *in = stdin;  
  for(int i = 0; i<numberElements; i++){
    readNextLine(in, 1, &h_in[i]);
  }


}
