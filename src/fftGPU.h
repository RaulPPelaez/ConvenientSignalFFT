/* Raul P. Pelaez 2020. Fast Fourier Transform cuFFT */
#ifndef FFTGPU_H
#define FFTGPU_H
#include<vector>
#include<cmath>
#include"defines.h"
#include"config.h"
#ifdef USE_CUDA
#include<cufft.h>
namespace gpu_mode{

  template<class T> struct cufftTypeAgnostic;
  template<> struct cufftTypeAgnostic<double>{using type=cufftDoubleReal;};
  template<> struct cufftTypeAgnostic<float>{using type=cufftReal;};
  template<class T> using cufftReal_t = typename cufftTypeAgnostic<T>::type;

  template<class T> struct cufftComplexType;
  template<> struct cufftComplexType<double>{using type=cufftDoubleComplex;};
  template<> struct cufftComplexType<float>{using type=cufftComplex;};
  template<class T> using cufftComplex_t = typename cufftComplexType<T>::type;
}

template<class floatType>
std::vector<gpu_mode::cufftComplex_t<floatType>> computeFFTWithCUFFT(const std::vector<floatType> &signal, Config config);
#else

#include<stdexcept>
namespace gpu_mode{
  struct dummytype2{float x,y;};
}
template<class floatType>
std::vector<gpu_mode::dummytype2> computeFFTWithCUFFT(std::vector<floatType> signal, Config config){
  (void)config;
  (void)signal;  
  throw std::runtime_error("This code was not compiled with GPU mode compatibility");  
}
#endif
#endif
