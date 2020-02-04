#ifndef FFTCONFIG_H
#define FFTCONFIG_H
#include"defines.h"
#include<string.h>
#include<sstream>
#include<iostream>
void print_help();

class CommandLineParser{
  int m_argc;
  char **m_argv;
public:
  enum OptionType {Required, Optional};
  CommandLineParser(int argc, char **argv): m_argc(argc), m_argv(argv){}
  
  std::istringstream getArgumentOfFlag(const char *flag,
				       OptionType type){
    for(int i=1; i<m_argc; i++){
      if(strcmp(flag, m_argv[i]) == 0){
	std::string line;
	if(i == m_argc-1){
	  break;
	}
	for(int j=i+1; j<m_argc; j++){	  
	  line += m_argv[j];
	  line += " ";
	}
	std::istringstream ss(line);
	return ss;
      }
    }
    if(type == Required)
      throw std::runtime_error(("Option " + std::string(flag) + " not found").c_str());
    return std::istringstream();
  }
  
};

enum class Device{cpu, gpu, def};
enum class Precision{single, dual, def};
enum class Normalization{amplitude, def};

struct Config{
  int numberElements;
  double Fs = 1;
  Device device = Device::def;
  Precision precision = Precision::def;
  std::string file = "/dev/stdin";
  Normalization normalization;
  void setDeviceFromString(std::string dev){
    if(dev.compare("cpu") == 0){
      device = Device::cpu;
    }
    else if(dev.compare("gpu") == 0){
      device = Device::gpu;
    }
    else{
      device = Device::def;
    } 
  }
  
  void setPrecisionFromString(std::string prec){
    if(prec.compare("float") == 0){
      precision = Precision::single;
    }
    else if(prec.compare("double") == 0){
      precision = Precision::dual;
    }
    else{
      precision = Precision::def;    
    }
  }
  
  void setNormalizationFromString(std::string norm){
    if(norm.compare("amplitude") == 0){
      normalization = Normalization::amplitude;
    }
    else{
      normalization = Normalization::def;    
    }
  }

  
};

Config TryToGetConfigurationFromCommandLine(int argc, char **argv);

Config getConfigurationFromCommandLine(int argc, char **argv);

#endif
