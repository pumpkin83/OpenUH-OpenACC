#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	long i;
	long N;
	int error;
	double TEST_VAL;
	//double *X;

	N = 1000000;
	TEST_VAL = 5;
	error = 0;
	double X[N];

	//X = (double*)malloc(N*sizeof(double));

	#pragma acc data copy(X[0:N])
	{
		for(i = 0; i < N; i++) 
			X[i] = TEST_VAL;

		#pragma acc update device(X[0:N])
	
		#pragma acc kernels param(X,N)
		{
		#pragma acc loop 
		for(i = 0; i < N; i++) 
			X[i] *= 2;
		}

		#pragma acc update host(X[0:N])
	
		for (i = 0; i < N; ++i) {
			if(X[i] != TEST_VAL*2)	
				error++;
		}
	
	}

	if(error == 0)
		printf("TEST PASS!\n");
	else
		printf("TEST FAILED!\n");

	return 0;
}

