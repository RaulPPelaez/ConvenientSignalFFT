



all:
	$(MAKE) -C src/
	mkdir -p bin
	mv src/a.out bin/fft 
