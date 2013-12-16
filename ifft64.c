/* ifft64.c   inverse fft  complex data stored re, im, re, im, ... */

void ifft64(float A[128])   /* complex vector, in and out A[0] to A[127] */
{
  float E[12] = /* constsnts for inverse fft */
   {-1.0, 0.0, 0.0, -1.0,
   0.707107, -0.707107, 0.92388, -0.382683,
   0.980785, -0.19509, 0.995185, -0.0980171};
  const int N = 64;
  const float FN = (float)N;
  float Tre, Tim;
  float WXre, WXim;
  float Wre, Wim;
  int   i=0;
  int   js=1;
  int   m;
  int   ix;
  int   isp;
  int   mmax=1;

  for(ix=1; ix<N; ix++)  /* reorder data */
  {
    if(js > ix)
    {
      Tre           = A[2*(js-1)];
      Tim           = A[2*(js-1)+1];
      A[2*(js-1)]   = A[2*(ix-1)];
      A[2*(js-1)+1] = A[2*(ix-1)+1];
      A[2*(ix-1)]   = Tre;
      A[2*(ix-1)+1] = Tim;
    }
    m = N / 2;
    while(m < js && m > 0)
    {
      js = js - m;
      m = m / 2;
    }
    js = js + m;
  }
  while(mmax < N)  /* compute transform */
  {
    isp = mmax + mmax;
    WXre = E[2*i];
    WXim = E[2*i+1];
    i++;
    Wre = 1.0;
    Wim = 0.0;
    for(m=0; m<mmax; m++)
    {
      ix = m;
      while(ix+mmax < N)
      {
        js = ix + mmax;
        Tre = Wre * A[2*js]   - Wim * A[2*js+1];
        Tim = Wre * A[2*js+1] + Wim * A[2*js];
        A[2*js]   = A[2*ix]   - Tre;  /* basic butterfly */
        A[2*js+1] = A[2*ix+1] - Tim;
        A[2*ix]   = A[2*ix]   + Tre;
        A[2*ix+1] = A[2*ix+1] + Tim;
        ix = ix + isp;
      }
      Tre = Wre * WXre - Wim * WXim;
      Wim = Wre * WXim + Wim * WXre;
      Wre = Tre;
    }
    mmax = isp;
  }
  /* only divide by N on inverse transform */
  for(i=0; i<N; i++)
  {
    A[2*i]   = A[2*i]   / FN;
    A[2*i+1] = A[2*i+1] / FN;
  }
} /* end ifft64.c */


