cmake_minimum_required (VERSION 3.8)

add_executable(ConvenientSignalFFT ConvenientSignalFFT.cpp config.cpp)
if(USE_CUDA)
  target_sources(ConvenientSignalFFT PRIVATE fftGPU.cu) 
  target_link_libraries(ConvenientSignalFFT cufft)
  include_directories(ConvenientSignalFFT_gpu   ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})
endif()

find_package(FFTW QUIET REQUIRED COMPONENTS SIMPLE)

if(NOT FFTW_FOUND OR USE_MKL)
  find_package(FFTW QUIET REQUIRED COMPONENTS MKL SIMPLE)
  find_package(MKL QUIET REQUIRED)
  include_directories(ConvenientSignalFFT ${FFTW_fftw3.h_DIRS})
  include_directories(ConvenientSignalFFT ${MKL_INCLUDE_DIR})
  target_link_libraries(ConvenientSignalFFT ${MKL_LIBRARIES})
  set(USE_MKL ON)
  if(NOT FFTW_FOUND)   
    set(USE_CPU OFF)
  endif()
endif()

if(USE_CPU)
  include_directories(ConvenientSignalFFT ${FFTW_INCLUDE_DIRS})
  link_directories(ConvenientSignalFFT ${FFTW_LIBRARY_DIRS})
  target_link_libraries(ConvenientSignalFFT  ${FFTW_LIBRARIES_DEP})
  target_sources(ConvenientSignalFFT PRIVATE fftCPU.cpp)
endif()

install(TARGETS ConvenientSignalFFT  RUNTIME DESTINATION bin)
  
configure_file ( "defines.h.in" ${CMAKE_CURRENT_SOURCE_DIR}/defines.h  )
