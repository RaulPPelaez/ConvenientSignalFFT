#include"fftCPU.h"
#include<cmath>
#include<fftw3.h>
#include<limits>
#include<algorithm>
//MKL wrappers do not have alloc functions...
#ifndef fftw_alloc_complex
#define NO_FFTW_ALLOC
#endif
namespace cpu_mode{
  template<class real> struct fftw_prec_types;
  template<> struct fftw_prec_types<double>{using type = fftw_complex;};
  template<> struct fftw_prec_types<float>{using type = fftwf_complex;};
  template<class real> using fftw_prec_types_t = typename fftw_prec_types<real>::type;
  
  template<class real>
  struct fftw_plan_prec;
  template<> struct fftw_plan_prec<double>{using type = fftw_plan;};
  template<> struct fftw_plan_prec<float>{using type = fftwf_plan;};  	      

  template<class real> typename fftw_prec_types<real>::type* fftw_alloc_complex_prec(int N);
  template<> typename fftw_prec_types<double>::type* fftw_alloc_complex_prec<double>(int N){
#ifdef NO_FFTW_ALLOC
    return (fftw_prec_types<double>::type*) malloc(N*sizeof(fftw_prec_types<double>::type));
#else	
    return fftw_alloc_complex(N);
#endif
  }
  template<> typename fftw_prec_types<float>::type* fftw_alloc_complex_prec<float>(int N){
#ifdef NO_FFTW_ALLOC
    return (fftw_prec_types<float>::type*) malloc(N*sizeof(fftw_prec_types<double>::type));
#else	
    return fftwf_alloc_complex(N);
#endif
  }
  
  template<class real>struct fftw_plan_dft_r2c_1d_prec;
  template<>struct fftw_plan_dft_r2c_1d_prec<double>{
    template <class ...T> typename fftw_plan_prec<double>::type operator()(T...args){return fftw_plan_dft_r2c_1d(args...);}};
  template<>struct fftw_plan_dft_r2c_1d_prec<float>{
    template <class ...T> typename fftw_plan_prec<float>::type operator()(T...args){return fftwf_plan_dft_r2c_1d(args...);}};

  void fftw_execute( fftw_plan_prec<double>::type plan){::fftw_execute(plan);}
  void fftw_execute( fftw_plan_prec<float>::type plan){::fftwf_execute(plan);}
}

template<class real>
std::vector<cpu_mode::real2<real>> computeFFTWithFFTW(std::vector<real> signal, Config config){
  using fftw_complex = cpu_mode::fftw_prec_types_t<real>;
  using fftw_plan = typename cpu_mode::fftw_plan_prec<real>::type;
  const int numberElements = config.numberElements;

  fftw_plan plan;
  real* in = signal.data();
  fftw_complex* out = cpu_mode::fftw_alloc_complex_prec<real>(numberElements/2+1);
  plan = cpu_mode::fftw_plan_dft_r2c_1d_prec<real>()(numberElements, in, out, FFTW_ESTIMATE);
  cpu_mode::fftw_execute(plan);  
  std::vector<cpu_mode::real2<real>> fft_real2(numberElements/2+1);
  std::transform(out, out + numberElements/2+1, fft_real2.begin(),
		 [](fftw_complex in){return cpu_mode::real2<real>{in[0], in[1]};});
  fftw_free(out);
  return std::move(fft_real2);
}

template std::vector<cpu_mode::real2<float>> computeFFTWithFFTW(std::vector<float> signal, Config config);

template std::vector<cpu_mode::real2<double>> computeFFTWithFFTW(std::vector<double> signal, Config config);
