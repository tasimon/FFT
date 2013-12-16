/* fft1_wav.c   */
/*
A 1D FFT 
Tyler Simon 8/14/2010
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fftc.h" /* needs fft64.c and ifft64.c */
 
#define N 64
static float A[2*N]; /* available for modifying transform */
static float Z[2*N];

int main(int argc, char * argv[])
{
  int i, k, sect, sectcnt, first;
  FILE * inp;
  FILE * outp;
  double sum, avg;
  double amplitude;
  char riff[4];
  int  sread, swrite; /* bytes read/written */
  int  fsize;
  char wave[4];
  char fmt[4];
  int  nbytes;
  short  ccode;
  short  channels;
  int rate;
  int avgrate; /* average rate in samples per second */
  short blockalign;
  short bps; /* bits per sample */
  char data[4];
  int csize;
  char stuf;
  char sbyte; /* byte of sound */
  int  ibyte;
  char more[4];
  int smin;
  int smax;
  int savg;
  int bad; /* flags bad data in read */
  int nbread; /* number of bytes read */
  
  printf("fft1_wave.c running \n");
  if(argc<2)
  {
    printf("no file to open, use sin_wav  train.wav \n");
    exit(1);
  }
  inp = fopen(argv[1], "rb");
  if(inp == NULL)
  {
    printf("can not open %s for reading. \n", argv[1]);
    exit(1);
  }
  if(argc<3)
  {
    printf("no output file given, opening junk.wav \n");
    outp = fopen("junk.wav","wb");
    if(outp == NULL)
    {
      printf("can not open junk.wav for writing. \n");
      exit(1);
    }
  }
  else /* open users output file */
  {
    outp = fopen(argv[2], "wb");
    if(outp == NULL)
    {
      printf("can not open %s for writing. \n", argv[2]);
      exit(1);
    }
  }
  printf("reading %s \n", argv[1]);

  sread = fread(&riff[0], 1, 4, inp);
  swrite = fwrite(&riff[0], 1, 4, outp);
  printf("first 4 bytes should be RIFF, <%c%c%c%c>\n",
         riff[0],riff[1],riff[2],riff[3]);

  sread = fread(&fsize, 1, 4, inp);
  swrite = fwrite(&fsize, 1, 4, outp);
  printf("file has %d +8 bytes \n", fsize);

  sread = fread(&wave[0], 1, 4, inp);
  swrite = fwrite(&wave[0], 1, 4, outp);
  printf("should be WAVE, <%c%c%c%c>\n",wave[0],wave[1],wave[2],wave[3]);

  sread = fread(&fmt[0], 1, 4, inp);
  swrite = fwrite(&fmt[0], 1, 4, outp);
  printf("should be fmt, <%c%c%c%c>\n",fmt[0],fmt[1],fmt[2],fmt[3]);

  sread = fread(&nbytes, 1, 4, inp);
  swrite = fwrite(&nbytes, 1, 4, outp);
  printf("block has %d more bytes \n", nbytes);

  sread = fread(&ccode, 1, 2, inp);
  swrite = fwrite(&ccode, 1, 2, outp);
  printf("compression code = %d \n", ccode);
  nbytes = nbytes-2;

  sread = fread(&channels, 1, 2, inp);
  swrite = fwrite(&channels, 1, 2, outp);
  printf("channels = %d \n", channels);
  nbytes = nbytes-2;

  sread = fread(&rate, 1, 4, inp);
  swrite = fwrite(&rate, 1, 4, outp);
  printf("rate = %d  \n", rate);
  nbytes = nbytes-4;

  sread = fread(&avgrate, 1, 4, inp);
  swrite = fwrite(&avgrate, 1, 4, outp);
  printf("avg rate = %d \n", avgrate);
  nbytes = nbytes-4;

  sread = fread(&blockalign, 1, 2, inp);
  swrite = fwrite(&blockalign, 1, 2, outp);
  printf("blockalign = %d  \n", blockalign);
  nbytes = nbytes-2;

  sread = fread(&bps, 1, 2, inp);
  swrite = fwrite(&bps, 1, 2, outp);
  printf("bits per sample = %d \n", bps);
  nbytes = nbytes-2;
  printf("bytes left in fmt = %d \n", nbytes);
  for(i=0; i<nbytes; i++)
  {
    sread = fread(&stuf, 1, 1, inp);
    swrite = fwrite(&stuf, 1, 1, outp);
  }

  sread = fread(&data[0], 1, 4, inp);
  swrite = fwrite(&data[0], 1, 4, outp);
  printf("should be data, <%c%c%c%c>\n",data[0],data[1],data[2],data[3]);

  sread = fread(&csize, 1, 4, inp);
  swrite = fwrite(&csize, 1, 4, outp);
  printf("chunk has %d more bytes \n", csize);
  nbread = 44+nbytes;
  printf("%d bytes read so far \n", nbread);
  
  bad = 0;
  savg = 0;
  sect = 0;
  sectcnt = 0;
  first = 1; /* control some debug print */

  for(i=0; i<csize; i++)
  {
    if(sect<N)
    {
      sread = fread(&sbyte, 1, 1, inp);
      if(sread != 1 && bad==0) { bad=1; printf("no read on byte %d \n", i); } 
      Z[2*sect] = (float)sbyte;
      Z[2*sect+1] = 0.0; /* no complex component */
      sect++;
    }
    else /* process N samples */
    {
      sect = 0;
      i--;
      sectcnt++;
      if(first)
      {
        printf("raw data \n");
        for(k=0; k<2*N; k++) A[k] = Z[k];
        for(k=0; k<N; k++)
		{ 
		printf("prefft k=%d, AR=%g, AI=%g \n",k,A[2*k],A[2*k+1]);
		}
        /*printf("k=%d, AR=%g, AI=%g \n",k,A[2*k],A[2*k+1]);*/
      }
      /* transform */
      fft64(Z); 
      if(first)
      {
        printf("transform \n");
        for(k=0; k<N; k++) 
		{
		printf("fft k=%d, AR=%g, AI=%g\n",k,Z[2*k],Z[2*k+1]);
		/*optional modifications */
		}
		
	}


	
	/*invert */
      ifft64(Z); 
      if(first)
      {
        printf("inverse transform \n");
        for(k=0; k<N; k++) printf("k=%d, AR=%g, AI=%g \n",k,Z[2*k],Z[2*k+1]);
        first = 0;
      }

      for(k=0; k<N; k++)
      {
       if(Z[2*k]>127.5) Z[2*k]=127.0;
       if(Z[2*k]<-127.5) Z[2]=-127.0;
        sbyte = (int)(Z[2*k]);
        swrite = fwrite(&sbyte, 1, 1, outp);
        ibyte = sbyte;
        savg = savg + ibyte;
        if(i==0) {smin=ibyte; smax=ibyte;}
        smin = ibyte<smin?ibyte:smin;
        smax = ibyte>smax?ibyte:smax;
        if(i<65 && k<20)
          printf("byte %d= %d  %d \n", k, sbyte, ibyte);
      }
    } /* end else part to write out transformed values */
  } /* end i<csize loop */
  /* just write leftovers, could find a smaller transform and pad */
  printf("writing %d unchanged bytes, after sector %d \n", sect, sectcnt);
  for(k=0; k<sect; k++)
  {
    sbyte = (int)(Z[2*k]);
    swrite = fwrite(&sbyte, 1, 1, outp);
  }
  savg = savg / csize;
  printf("smin=%d, smax=%d, savg=%d \n", smin, smax, savg);

  nbread = nbread+csize;
  printf("%d bytes read so far \n", nbread);

  while(1) /* just copy remaining chunks */
  {
    sread = fread(&more[0], 1, 4, inp);
    if(sread != 4) goto done;
    swrite = fwrite(&more[0], 1, 4, outp);
    printf("could be , <%c%c%c%c>\n",more[0],more[1],more[2],more[3]);

    sread = fread(&csize, 1, 4, inp); /* check for more chunks */
    if(sread != 4) goto done;
    swrite = fwrite(&csize, 1, 4, outp);
    printf("chunk has %d bytes \n", csize);
    for(i=0; i<csize; i++)
    {
      sread = fread(&sbyte, 1, 1, inp);
      if(sread != 1) goto done;
      swrite = fwrite(&sbyte, 1, 1, outp);
    }
    nbread = nbread + 8 + csize;
    printf("%d bytes read so far \n", nbread);
  } /* end copying remaining chunks */
  
done: ;
  fclose(inp);
  fflush(outp);
  fclose(outp);
  if(argc<3) printf("fft1_wave done. new junk.wav file written \n");
  else  printf("fft1_wave done. new %s file written \n", argv[2]);
  return 0;
} /* end fft1_wave .c */
