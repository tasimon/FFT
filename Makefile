
all: fft_test

fft_test: fft1_wav.c fft64.c ifft64.c 
	cc -o fft_test fft1_wav.c fft64.c ifft64.c -I./

clean: 
	rm fft_test 
