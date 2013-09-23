#include <stdio.h>
#include <stdlib.h>
#include <accelmath.h>

int main(int argc, char** argv)
{
	float *a, *b;
	int i, N;
	float sum;

	N = 4096;

	a = (float*)malloc(N*sizeof(float));
	b = (float*)malloc(N*sizeof(float));

	srand(1234);
	for(i=0; i<N; i++)
	{
		a[i] = rand()/(float)RAND_MAX;
		b[i] = 0.0;
	}

	sum = 0;
  #pragma acc data copyin(a[0:N])
  {
    #pragma acc kernels loop independent
	for(i=0; i<N; i++)
	{
		//b[i] =  sinf(a[i])*sinf(a[i]) +
		//	    cosf(a[i])*cosf(a[i]);
		b[i] =  sinf(a[i]) + cosf(a[i]+1);
	}
  }

	free(a); free(b);
	//printf("Sum = %f\n", sum);
	return 0;
}

