/* Raul P. Pelaez 2017. Fast Fourier Transform cuFFT */

#include<iostream>

#include<vector>

#include<cufft.h>
#include<thrust/device_vector.h>
#include<thrust/host_vector.h>
#include<thrust/extrema.h>
#include<cmath>
#include"vector_algebra.h"
#include"superRead.h"



//Transforming cufft macros to templates...

template<class T> struct cufftTypeAgnostic;
template<> struct cufftTypeAgnostic<double>{using type=cufftDoubleReal;};
template<> struct cufftTypeAgnostic<float>{using type=cufftReal;};
template<class T> using cufftReal_t = typename cufftTypeAgnostic<T>::type;

template<class T> struct cufftComplexType;
template<> struct cufftComplexType<double>{using type=cufftDoubleComplex;};
template<> struct cufftComplexType<float>{using type=cufftComplex;};
template<class T> using cufftComplex_t = typename cufftComplexType<T>::type;

template<class T> struct CUFFT_Real2Complex;
template<> struct CUFFT_Real2Complex<double>{static constexpr cufftType value=CUFFT_D2Z;};
template<> struct CUFFT_Real2Complex<float>{static constexpr cufftType value=CUFFT_R2C;};




template<class floatType>
void cufftExecReal2Complex(cufftHandle &plan,
			   cufftReal_t<floatType>* d_in,
			   cufftComplex_t<floatType> *d_out);
template<>
void cufftExecReal2Complex<float>(cufftHandle &plan,
				  cufftReal_t<float>* d_in,
				  cufftComplex_t<float> *d_out){
cufftExecR2C(plan, d_in, d_out);
}

template<>
void cufftExecReal2Complex<double>(cufftHandle &plan,
				   cufftReal_t<double>* d_in,
				   cufftComplex_t<double> *d_out){

  cufftExecD2Z(plan, d_in, d_out);
}

template<class floatType>
void FFTCudaMode( int numberElements, double Fs){

  using cufftComplex_t = cufftComplex_t<floatType>;
  using cufftReal_t = cufftReal_t<floatType>;

  cufftHandle plan;
  thrust::device_vector<cufftComplex_t> data(numberElements/2+1);

  thrust::host_vector<cufftComplex_t> h_data(numberElements/2+1, cufftComplex_t());

  cufftReal_t *h_in = (cufftReal_t*) thrust::raw_pointer_cast(h_data.data());
  FILE *in = stdin;  
  for(int i = 0; i<numberElements; i++){
    readNextLine(in, 1, &h_in[i]);
  }
  //Upload
  data = h_data;

  //Create and execute cuFFT plan


  cufftComplex_t *d_m = thrust::raw_pointer_cast(data.data());

  cufftPlan1d(&plan, numberElements, CUFFT_Real2Complex<floatType>::value, 1);
  cufftExecReal2Complex<floatType>(plan, (cufftReal_t*) d_m, d_m);


  cudaDeviceSynchronize();

  //Download
  h_data = data;


  thrust::device_vector<floatType> devAmplitudes(numberElements/2+1);
  
  //Print
  thrust::transform(data.begin(),
		    data.begin()+numberElements/2+1,
		    devAmplitudes.begin(),
		    [=] __device__ (cufftComplex_t a){ return 2.0*sqrt(a.x*a.x+a.y*a.y)/(floatType)numberElements;}
		    );

  floatType maxAmplitude = *thrust::max_element(devAmplitudes.begin(),
					     devAmplitudes.end());
  
  thrust::device_vector<floatType> hostAmplitudes = devAmplitudes;

  
  for(int i = 0; i<numberElements/2+1; i++){

    floatType Aw = hostAmplitudes[i];    
    floatType phase = 0.0;
    if(Aw >= maxAmplitude/1000.0)
      phase = std::atan2(h_data[i].y, h_data[i].x);
    
    
    std::cout<<i*Fs/(numberElements)<<" "<<Aw<<" "<<phase<<"\n";
  }
  std::cout<<std::flush;

  cudaDeviceSynchronize();

}
