#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
	int i;
	int *A, *B, *C;
	int error;
	int N, TESTVAL;

	N = 1000;
	TESTVAL = 34;

	A = (int*) malloc(N * sizeof(int));
	B = (int*) malloc(N * sizeof(int));
	C = (int*) malloc(N * sizeof(int));
	
	error = 0;

#pragma acc data create(A[0:N],B[0:N]) copyout(C[0:N])
{
	#pragma acc kernels param(A,B)
	{
		#pragma acc loop
		for (i = 0; i < N; i++) 
		{
			A[i] = TESTVAL;
			B[i] = TESTVAL;
		}
	}
	
	#pragma acc kernels param(A,B,C)
	{
	#pragma acc loop
	for (i = 0; i < N; i++) 
		C[i] = A[i] + B[i];
	}
}

	for (i = 0; i < N; ++i) {
		if(C[i] != TESTVAL*2)
			error++;
	}
	
	free(A);
	free(B);
	free(C);
	
	if(error == 0)
		printf("TEST PASS!\n");
	else
		printf("TEST FAILED!\n");


	return 0;
}

