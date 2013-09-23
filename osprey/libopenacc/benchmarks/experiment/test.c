#include <stdio.h>
#include <stdlib.h>

#define N 1024

float a[N][N];
float b[N][N];
float c[N][N];

float get_sum()
{
	int i, j;
	float sum = 0.0f;
	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
			sum += c[i][j];
	}
	return sum;
}

int main()
{
	int i,j,k;
	float sum;

	srand(1234);


	for(i=0; i<N; i++)
	{
		for(j=0; j<N; j++)
		{
			a[i][j] = rand()/(float)RAND_MAX;
			b[i][j] = rand()/(float)RAND_MAX;
			c[i][j] = 0.0;
		}
	}

//#pragma acc kernels copyin(a[0:N][0:N], b[0:N][0:N]) copy(c[0:N][0:N])
#pragma acc kernels copyin(a[0:N*N], b[0:N*N]) copy(c[0:N*N]) async(0) 
{
    #pragma acc loop gang vector
	for(i=0; i<N; i++)
	{
	    #pragma acc loop gang vector
		for(j=0; j<N; j++)
		{
			for(k=0; k<N; k++)
				c[i][j] += a[i][k]*b[k][j];
		}
	}
}
	sum = get_sum(c);
	printf("Sum is: %f\n", sum);

	return 0;
}
