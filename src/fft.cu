/* Raul P. Pelaez 2017. Fast Fourier Transform

   Takes a single column signal and outputs its FFT (using cuda) in frequency domain.
   The output format is:
      k Amplitude(k) phase(k)
      
Usage:

fft -N [N] -Fs [Fs] -prec [double/float] < signal

N: Number of points in the signal
Fs: Sampling frequency
prec: Precision mode, float by default

Example:

seq 0 0.2 10000 |  awk '{print sin($1)}' | fft -N 10000 -Fs 5.0 > kk

w=$(grep $( datamash -W max 2 <kk) kk | awk '{print 2*3.1415*$1}')

w will be 1

 */
#include<iostream>

#include<vector>

#include"fftGPU.cuh"

//#include"fftCPU.h"

#include<cmath>
#include<sstream>

//Ask for one or many arguments of any type after a flag from argv
template<class T>
bool parseArgument(int argc, char *argv[],
		   const char *flag,  //What to look for
		   T* result, int numberArguments=1){ //output and number of them
  for(int i=1; i<argc; i++){ //Look for the flag

    if(strcmp(flag, argv[i]) == 0){ //If found it
      std::string line;  //convert char * to string for as many values as requested
      for(int j=0; j<numberArguments; j++)
	line += argv[i+j+1];

      std::istringstream ss(line);
      //Store them in result
      for(int j=0; j<numberArguments; j++){
	ss>>result[j];
      }
      return true;
    }
  }
  return false;
}


void print_help();
int main(int argc, char *argv[]){
  if(argc<3){
    print_help();
    exit(1);
  }
  
  int numberElements;
  if(!parseArgument(argc, argv, "-N", &numberElements)){
    print_help();
    exit(1);
  }
    
  double Fs;
  if(!parseArgument(argc, argv, "-Fs", &Fs)){
    print_help();
    exit(1);
  }

  std::string prec = "float";
  parseArgument(argc, argv, "-prec", &prec);

  std::string mode = "GPU";
  parseArgument(argc, argv, "-mode", &mode);

  if(prec.compare("float")==0){
    if(mode.compare("GPU")==0){      
      FFTCudaMode<float>(numberElements, Fs);
    }
    else if(mode.compare("CPU")==0){
    }
    else{
      std::cerr<<"Unrecognized mode"<<std::endl;
      print_help();
      exit(1);
    }
  }
  else if(prec.compare("double")==0){
    if(mode.compare("GPU")==0){
      FFTCudaMode<double>(numberElements, Fs);
    }
    else if(mode.compare("CPU")==0){
    }
    else{
      std::cerr<<"Unrecognized mode"<<std::endl;
      print_help();
      exit(1);
    }	
  }
  else{
    std::cerr<<"Unrecognized prec"<<std::endl;
    print_help();
    exit(1);    
  }
  
   
 return 0;
}


void print_help(){
std::cerr<<"ERROR!!: Input missing"<<std::endl;
    std::cerr<<"Takes a single column signal and outputs its FFT (using cuda) in frequency domain."<<std::endl;
    std::cerr<<"The output format is:"<<std::endl;
    std::cerr<<"\tk Amplitude(k) phase(k)"<<std::endl;

    std::cerr<<""<<std::endl;
    std::cerr<<"Usage:"<<std::endl;
    std::cerr<<""<<std::endl;
    std::cerr<<"fft -N [N] -Fs [Fs] -prec [float/double] < signal"<<std::endl;
    std::cerr<<""<<std::endl;
    std::cerr<<"N: Number of points in the signal"<<std::endl;
    std::cerr<<"Fs: Sampling frequency"<<std::endl;
    std::cerr<<"prec: Precision mode, double by default"<<std::endl;
    std::cerr<<""<<std::endl;
    std::cerr<<"Example:"<<std::endl;
    std::cerr<<""<<std::endl;
    std::cerr<<"seq 0 0.2 10000 |  awk '{print sin($1)+rand()-0.5}' | fft -N 10000 -Fs 5.0 > kk"<<std::endl;
    std::cerr<<""<<std::endl;
    std::cerr<<"w=$(grep $( datamash -W max 2 <kk) kk | awk '{print 2*3.1415*$1}')"<<std::endl;
    std::cerr<<std::endl;
    std::cerr<<"w will be close to 1"<<std::endl;    
}
