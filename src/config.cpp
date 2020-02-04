#include"config.h"

Config TryToGetConfigurationFromCommandLine(int argc, char **argv){
  CommandLineParser parser(argc, argv);
  Config config;
  parser.getArgumentOfFlag("-N", CommandLineParser::Required)>>config.numberElements;
  parser.getArgumentOfFlag("-Fs", CommandLineParser::Optional)>>config.Fs;
  parser.getArgumentOfFlag("-file", CommandLineParser::Optional)>>config.file;
  std::string dev;
  parser.getArgumentOfFlag("-mode", CommandLineParser::Optional)>>dev;
  config.setDeviceFromString(dev);
  std::string prec;
  parser.getArgumentOfFlag("-prec", CommandLineParser::Optional)>>prec;
  config.setPrecisionFromString(prec);
  std::string norm;
  parser.getArgumentOfFlag("-normalization", CommandLineParser::Optional)>>norm;
  config.setNormalizationFromString(norm);
  return config;
}

Config getConfigurationFromCommandLine(int argc, char **argv){
  Config config;
 try{
    config = TryToGetConfigurationFromCommandLine(argc, argv);
  }
  catch(...){
    print_help();
    throw;
  }
 return config;
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
