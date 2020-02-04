## **Raul P. Pelaez 2020. Fast Fourier Transform**

Takes a single column signal and outputs its FFT (using CUDA, FFTW or MKL) in frequency domain.  

GPU mode needs CUDA>7.5 to work, if you have boost you can activate it in CMAKE and gain some I/O speed.  

## Compilation:
Use CMake:  

```bash
mkdir build
cd build
cmake ../
make
cd bin
```
	
## Usage:

fft -N [N] -Fs [Fs] -prec [double/float] -mode [gpu/cpu/default] -normalization [amplitude/default] < signal  

N: Number of points in the signal  
Fs: Sampling frequency  
prec: Precision mode, float by default  
mode: Perform the FFT in GPU or CPU  
normalization: Postprocess of the FFT output. Amplitude outputs abs(fft[i]) and atan2(real(fft[i])/imag(fft[i])), default just divides by N.  

### Example:
Create a cos signal with a single frequency, perform the fft and get the frequency of the peak (which should be this same frequency.  
```bash
L=16384
Fs=16.384
freq=3

seq $L | awk '{pi=atan2(0,-1); print cos('$freq'*2.0*pi*$1/'$Fs')}' |
         fft -N $L -Fs $Fs -normalization amplitude |
	 awk '$2>maxAmpl{maxAmpl=$2;freq=$1}END{print freq}'

```
The output will be $freq.  
