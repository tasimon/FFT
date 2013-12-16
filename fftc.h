/* fftc.h          header file for various fft's and inverses */
void  fft8  (short int Z[16]);   /* standard complex FFT */

void  fft16  (float Z[32]);   /* standard complex FFT */
void ifft16  (float Z[32]);   /* inverse fft          */
void  fft32  (float Z[64]);   /* standard complex FFT */
void ifft32  (float Z[64]);   /* inverse fft          */
void  fft64  (float Z[128]);  /* standard complex FFT */
void ifft64  (float Z[128]);  /* inverse fft          */
void  fft128 (float Z[256]);  /* standard complex FFT */
void ifft128 (float Z[256]);  /* inverse fft          */
void  fft256 (float Z[512]);  /* standard complex FFT */
void ifft256 (float Z[512]);  /* inverse fft          */
void  fft512 (float Z[1024]); /* standard complex FFT */
void ifft512 (float Z[1024]); /* inverse fft          */
void  fft1024(float Z[2048]); /* standard complex FFT */
void ifft1024(float Z[2048]); /* inverse fft          */
void  fft2048(float Z[4096]); /* standard complex FFT */
void ifft2048(float Z[4096]); /* inverse fft          */
void  fft4096(float Z[8192]); /* standard complex FFT */
void ifft4096(float Z[8192]); /* inverse fft          */



