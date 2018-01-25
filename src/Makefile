


#Uncomment this line if boost is present in the system, it will speed the runtime considerably
#USE_BOOST=-DUSE_BOOST

all:
	nvcc --expt-extended-lambda  --std=c++11 -O3 fft.cu -DCUDA_ENABLED $(USE_BOOST) -lcufft
