
/* Raul P. Pelaez 2020. Fast Fourier Transform

   Takes a single column signal and outputs its FFT (using cuda) in frequency domain.
   The output format is:
      k Amplitude(k) phase(k)
      
Usage:

fft -N [N] -Fs [Fs] -prec [double/float] < signal

N: Number of points in the signal
Fs: Sampling frequency
prec: Precision mode, float by default

Example:

Create a cos signal with a single frequency, perform the fft and get the frequency of the peak (which should be this same frequency.

L=16384
Fs=16.384
freq=3

seq $L | awk '{pi=atan2(0,-1); print cos('$freq'*2.0*pi*$1/'$Fs')}' |
         fft -N $L -Fs $Fs -normalization amplitude |
	 awk '$2>maxAmpl{maxAmpl=$2;freq=$1}END{print freq}'

This should print $freq 

 */
#include<iostream>
#include<vector>
#include"third_party/superio/superIO.h"
#include"defines.h"
#include"config.h"
#include"fftGPU.h"
#include"fftCPU.h"
#include<cmath>

template<class FloatType>
std::vector<FloatType> readSignalFromFile(Config config){
  std::vector<FloatType> signal(config.numberElements);

  superIO::superInputFile file("/dev/stdin");

  for(int i = 0; i<config.numberElements; i++){    
    if(!file.good()){
      throw std::runtime_error("Input ended before expected");
    }
    char* line;
    int numberChars = file.getNextLine(line);
    superIO::string2numbers(line, numberChars, 1, &signal[i]);
  }
  return std::move(signal);
}

template<Device dev, class FloatType> struct DispatchComputeFFT;

template<class FloatType>
struct DispatchComputeFFT<Device::gpu, FloatType>{
  static auto dispatch(std::vector<FloatType> &signal, Config config){
    auto res = computeFFTWithCUFFT(signal, config);
    return std::move(res);
  }
};

template<class FloatType>
struct DispatchComputeFFT<Device::cpu, FloatType>{
  static auto dispatch(std::vector<FloatType> &signal, Config config){
    auto res = computeFFTWithFFTW(signal, config);
    return std::move(res);
  }
};

template<class ComplexType>
void normalizeFFT(std::vector<ComplexType> &fft, Config config){
  using floatType = decltype(fft[0].x);
  for(int i = 0; i<config.numberElements/2+1; i++){
    auto tmp = fft[i];
    floatType prefactor = 1.0/config.numberElements;
    if(config.normalization == Normalization::amplitude){
      floatType Aw = 2.0*prefactor*sqrt(tmp.x*tmp.x+tmp.y*tmp.y);    
      floatType phase = std::atan2(tmp.y, tmp.x);
      fft[i] = {Aw, phase};
    }
    else{
      fft[i] = {tmp.x*prefactor, tmp.y*prefactor};
    }
  }
}

template<class ComplexType>
void writeFFTToFile(const std::vector<ComplexType> &fft, Config config){
  using floatType = decltype(fft[0].x);
  superIO::superOutputFile out(stdout);
  std::string to_write;
  for(int i = 0; i< config.numberElements/2+1; i++){
    floatType frequency = i*config.Fs/config.numberElements;
    floatType amplitude = fft[i].x;
    floatType phase = fft[i].y;    
    to_write.clear();
    superIO::number2string(to_write, frequency);
    to_write += " ";
    superIO::number2string(to_write, amplitude);
    to_write += " ";
    superIO::number2string(to_write, phase);
    to_write +="\n";
    out.write(to_write.c_str(), to_write.size());
  }
  out.flush();
}


template<Device dev, class FloatType> void dispatchRunFFT(Config config){
  auto signal = readSignalFromFile<FloatType>(config);
  auto fft = DispatchComputeFFT<dev, FloatType>::dispatch(signal, config);
  normalizeFFT(fft, config);
  writeFFTToFile(fft, config);
}


template<class FloatType>
void runFFT(Config config){
  if(config.device == Device::gpu){
    dispatchRunFFT<Device::gpu, FloatType>(config);
  }
  else if(config.device == Device::cpu or config.device == Device::def){
    dispatchRunFFT<Device::cpu, FloatType>(config);
  }

}

int main(int argc, char *argv[]){
  Config config = getConfigurationFromCommandLine(argc, argv);
  if(config.precision == Precision::single){  
    runFFT<float>(config);
  }
  else{
    runFFT<double>(config);
  }   
 return 0;
}

