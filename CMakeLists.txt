cmake_minimum_required (VERSION 3.8)
include(CheckLanguage)
include(CheckCXXCompilerFlag)
project (ConvenientSignalFFT CXX)
#set( CMAKE_VERBOSE_MAKEFILE on )
set (ConvenientSignalFFT_VERSION_MAJOR 1)
set (ConvenientSignalFFT_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

#I want only std=c++11, not c++0x
#check_cxx_compiler_flag("-std=c++14" HAS_CXX11)
#if(NOT HAS_CXX11)
#  message(FATAL_ERROR "C++ compiler needs to allow c++11 standard")
#endif()

option(USE_BOOST "Use Boost-qi for reading, highly improves reading performance" OFF)
option(USE_MKL "Uses MKL FFTW interface instead of FFTW3" OFF)
option(DONT_USE_CUDA "Dont compile in hybrid CPU/GPU mode, requires nvcc" OFF)
option(DONT_USE_CPU "Dont compile for FFTW" OFF)
if(NOT DONT_USE_CPU)
  set(USE_CPU ON)
endif()

if(NOT DONT_USE_CUDA)
  check_language(CUDA)
  if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    set(USE_CUDA ON)
    
    set(CMAKE_CUDA_STANDARD 11)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    set(CMAKE_CUDA_SEPARABLE_COMPILATION OFF)
    
    execute_process(COMMAND bash  ${PROJECT_SOURCE_DIR}/cmake/listArchs.sh OUTPUT_VARIABLE ARCHS)
    set(CMAKE_CUDA_FLAGS ${ARCHS} ${CMAKE_CUDA_FLAGS}) 
  endif()
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC" ${CMAKE_CXX_FLAGS})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)




add_subdirectory(src)

