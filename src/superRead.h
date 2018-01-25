#ifndef SUPER_READ_H
#define SUPER_READ_H
#include<string>
#include<cstdlib>
#include<stdio.h>

#ifdef USE_BOOST
#include <boost/spirit/include/qi_real.hpp>
namespace qi = boost::spirit::qi;
#endif



template<class floatType>
inline void readNextLineBoost(FILE *in, int ncols, floatType *readedValues){
#ifdef USE_BOOST
  static char *line = nullptr;
  static size_t linesize;
  int nCharacters = getline(&line, &linesize, in);
  int currentCharacter = 0;
  int firstCharacter = 0; //First character iof a number
  for(int i=0; i<ncols; i++){
 
    while(!std::isspace(line[currentCharacter]) && currentCharacter<nCharacters){currentCharacter++;}    
    
    double value;    
    qi::parse(&line[firstCharacter], line+currentCharacter , qi::double_, value);
    readedValues[i] = value;
    
    currentCharacter++;
    firstCharacter = 0;
    
  }
#else

  std::cerr<<"ERROR: Boost support not enabled during compilation!!!"<<std::endl;
  exit(1);

#endif

  
}

template<class floatType>
inline void readNextLineSTD(FILE *in, int ncols, floatType *readedValues){
  static char *line = nullptr;
  static size_t linesize;
  
  int nr = getline(&line, &linesize, in);
  
  char *l2;
  char *l1 = line;

  for(int i=0; i<ncols; i++){
    readedValues[i] = strtod(l1, &l2); l1=l2;
  }
 
}

template<class floatType>
inline void readNextLine(FILE *in, int ncols, floatType *readedValues){

  #ifdef USE_BOOST
  readNextLineBoost(in, ncols, readedValues);
  #else
  readNextLineSTD(in, ncols, readedValues);
  #endif
 
}
#endif
