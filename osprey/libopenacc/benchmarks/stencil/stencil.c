
/***************************************************************************
 *cr
 *cr            (C) Copyright 2010 The Board of Trustees of the
 *cr                        University of Illinois
 *cr                         All Rights Reserved
 *cr
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include <openacc_rtl.h>
#ifdef __OPENACC
	#include <openacc.h>
#endif

#define infile "/home/rengan/openacc-test-suite/certification_suite/PARBOIL/parboil/datasets/stencil/default/input/512x512x64x100.bin"
#define outfile "out.dat"

#define Index3D(_nx,_ny,_i,_j,_k) ((_i)+_nx*((_j)+_ny*(_k)))

static int read_data(float *A0, int nx,int ny,int nz,FILE *fp) 
{	
	int s=0;
        int i, j, k;
	for(i=0;i<nz;i++)
	{
		for(j=0;j<ny;j++)
		{
			for(k=0;k<nx;k++)
			{
                fread(A0+s,sizeof(float),1,fp);
				s++;
			}
		}
	}
	return 0;
}

void cpu_stencil(float c0,float c1, float *A0,float * Anext,const int nx, const int ny, const int nz)
{

  int i, j, k;
  #pragma acc data present(A0[0:nx*ny*nz], Anext[0:nx*ny*nz])
  {
    //#pragma acc kernels param(A0,Anext,nx,ny,nz,c0,c1)
    #pragma acc kernels
	{
	//#pragma acc loop independent gang(510)
	//#pragma acc loop independent vector(128)
	#pragma acc loop independent vector(2)
	for(i=1;i<nx-1;i++)
	{ 
	    #pragma acc loop independent gang(255) vector(4)
	    //#pragma acc loop independent gang(255) vector(2)
	    //#pragma acc loop independent gang vector
		for(j=1;j<ny-1;j++)
		{
		   #pragma acc loop independent gang(16) vector(64)
		   //#pragma acc loop independent vector(128)
		   //#pragma acc loop independent gang(62)
		   //#pragma acc loop independent gang vector
			for(k=1;k<nz-1;k++)
			{
				Anext[Index3D (nx, ny, i, j, k)] = 
				(A0[Index3D (nx, ny, i, j, k + 1)] +
				A0[Index3D (nx, ny, i, j, k - 1)] +
				A0[Index3D (nx, ny, i, j + 1, k)] +
				A0[Index3D (nx, ny, i, j - 1, k)] +
				A0[Index3D (nx, ny, i + 1, j, k)] +
				A0[Index3D (nx, ny, i - 1, j, k)])*c1
				- A0[Index3D (nx, ny, i, j, k)]*c0;
			}
		}
	}
	}
  }
}

void outputData(char* fName, float *h_A0,int nx,int ny,int nz)
{
  FILE* fid = fopen(fName, "w");
  uint32_t tmp32;
  if (fid == NULL)
    {
      fprintf(stderr, "Cannot open output file\n");
      exit(-1);
    }
  tmp32 = nx*ny*nz;
  fwrite(&tmp32, sizeof(uint32_t), 1, fid);
  fwrite(h_A0, sizeof(float), tmp32, fid);

  fclose (fid);
}

int main(int argc, char** argv) {
	printf("OpenACC-based 7 points stencil codes****\n");
	printf("Original version by Rengan Xu <uhxrg@cs.uh.edu>\n");
	printf("This version maintained by Rengan Xu  ***********\n");

	//declaration
	int nx,ny,nz;
	int size;
    int iteration;
	float c0=1.0f/6.0f;
	float c1=1.0f/6.0f/6.0f;
	struct timeval tim;
	double start, end;

	if (argc<5) 
    {
      printf("Usage: probe nx ny nz tx ty t\n"
	     "nx: the grid size x\n"
	     "ny: the grid size y\n"
	     "nz: the grid size z\n"
		  "t: the iteration time\n");
      return -1;
    }

	nx = atoi(argv[1]);
	if (nx<1)
		return -1;
	ny = atoi(argv[2]);
	if (ny<1)
		return -1;
	nz = atoi(argv[3]);
	if (nz<1)
		return -1;
	iteration = atoi(argv[4]);
	if(iteration<1)
		return -1;

	//host data
	float *h_A0;
	float *h_Anext;
	float *temp;

	size=nx*ny*nz;
	
	h_A0=(float*)malloc(sizeof(float)*size);
	h_Anext=(float*)malloc(sizeof(float)*size);
  FILE *fp = fopen(infile, "rb");
	read_data(h_A0, nx,ny,nz,fp);
  fclose(fp);
  memcpy (h_Anext,h_A0 ,sizeof(float)*size);


  int t;
  acc_init(acc_device_default);

  gettimeofday(&tim, NULL);
  start = tim.tv_sec + tim.tv_usec/1000000.0;
#pragma acc data copy(h_A0[0:size]) copyin(h_Anext[0:size])
{
	for(t=0;t<iteration;t++)
	{
		cpu_stencil(c0,c1, h_A0, h_Anext, nx, ny,  nz);
    	temp=h_A0;
    	h_A0 = h_Anext;
    	h_Anext = temp;
	}
}
  gettimeofday(&tim, NULL);
  end = tim.tv_sec + tim.tv_usec/1000000.0;
	outputData(outfile, h_A0, nx, ny, nz);
 
 	printf("The stencil execution time: %.2f s\n", end-start);
		
	free (h_A0);
	free (h_Anext);

	return 0;
}
