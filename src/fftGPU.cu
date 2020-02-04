/* Raul P. Pelaez 2020. Fast Fourier Transform cuFFT */
#include"fftGPU.h"
#include<cufft.h>
#include<thrust/device_vector.h>

namespace gpu_mode{
  
  template<class T> struct CUFFT_Real2Complex;
  template<> struct CUFFT_Real2Complex<double>{static constexpr cufftType value=CUFFT_D2Z;};
  template<> struct CUFFT_Real2Complex<float>{static constexpr cufftType value=CUFFT_R2C;};

  template<class floatType>
  void cufftExecReal2Complex(cufftHandle &plan, cufftReal_t<floatType>* d_in, cufftComplex_t<floatType> *d_out);
  
  template<>
  void cufftExecReal2Complex<float>(cufftHandle &plan, cufftReal_t<float>* d_in, cufftComplex_t<float> *d_out){
    cufftExecR2C(plan, d_in, d_out);
  }

  template<>
  void cufftExecReal2Complex<double>(cufftHandle &plan, cufftReal_t<double>* d_in, cufftComplex_t<double> *d_out){
    cufftExecD2Z(plan, d_in, d_out);
  }
}

template<class floatType>
std::vector<gpu_mode::cufftComplex_t<floatType>> computeFFTWithCUFFT(const std::vector<floatType> &signal, Config config){
  using cufftComplex_t = gpu_mode::cufftComplex_t<floatType>;
  using cufftReal_t = gpu_mode::cufftReal_t<floatType>;
  thrust::device_vector<cufftComplex_t> data(config.numberElements/2+1);
  cufftComplex_t* d_m = thrust::raw_pointer_cast(data.data());
  thrust::device_ptr<floatType> d_data((floatType*) d_m);
  thrust::copy(signal.begin(), signal.end(), d_data);
  cufftHandle plan;
  cufftPlan1d(&plan, config.numberElements, gpu_mode::CUFFT_Real2Complex<floatType>::value, 1);
  gpu_mode::cufftExecReal2Complex<floatType>(plan, (cufftReal_t*) d_m, d_m);
  std::vector<cufftComplex_t> fft(config.numberElements/2+1);
  thrust::copy(data.begin(), data.end(), fft.begin());
  return std::move(fft);
}

template std::vector<gpu_mode::cufftComplex_t<float>> computeFFTWithCUFFT(const std::vector<float> &signal, Config config);
template std::vector<gpu_mode::cufftComplex_t<double>> computeFFTWithCUFFT(const std::vector<double> &signal, Config config);
