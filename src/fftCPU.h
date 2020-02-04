/* Raul P. Pelaez 2019. Fast Fourier Transform FFTW */
#ifndef FFTCPU_H
#define FFTCPU_H
#include"defines.h"
#include"config.h"
#include<vector>
#ifdef USE_CPU
namespace cpu_mode{
  template<class real>
  struct real2{
    real x,y;
  };
}

template<class floatType>
std::vector<cpu_mode::real2<floatType>> computeFFTWithFFTW(std::vector<floatType> signal, Config config);

#else

namespace cpu_mode{
  struct dummytype2{float x,y;};
}

template<class floatType>
std::vector<cpu_mode::dummytype2> computeFFTWithFFTW(std::vector<floatType> signal, Config config){
  (void)config;
  (void)signal;  
  throw std::runtime_error("This code was not compiled with CPU mode compatibility");  
}

#endif

#endif
