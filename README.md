## **Raul P. Pelaez 2017. Fast Fourier Transform**

Takes a single column signal and outputs its FFT (using cuda) in frequency domain.  

Needs CUDA>7.5 to work, check src/Makefile, if you have boost you can say so there and gain some speed.  

## Usage:

fft [N] [Fs] -prec [double/float] < signal  

N: Number of points in the signal  
Fs: Sampling frequency  
prec: Precision mode, float by default  

### Example:

```bash
seq 0 0.2 10000 |  awk '{print sin($1)}' | fft 10000 5.0 > kk

w=$(grep $( datamash -W max 2 <kk) kk | awk '{print 2*3.1415*$1}')

```
w will be 1
