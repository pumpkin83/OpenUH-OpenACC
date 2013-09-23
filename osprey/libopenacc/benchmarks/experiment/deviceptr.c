#include <stdio.h>
#include <stdlib.h>
#include <openacc_rtl.h>

int main()
{
	int i, N, sum, known_sum;
	int *A, *B;
	N = 4096;

	A = (int*)acc_malloc(sizeof(int)*N);
	B = (int*)malloc(N*sizeof(B));

	acc_init(acc_device_not_host);

#pragma acc data deviceptr(A) copyout(B[0:N])
{
   #pragma acc kernels loop gang(8) vector(N/8)
	for(i=0; i<N; i++)
	{
		A[i] = i+1;
		B[i] = A[i];
	}
}

	sum = 0;
	for(i=0; i<N; i++)
	{
		sum += B[i];
	}

	acc_free(A);

	known_sum = N*(N-1)/2 + N;
	if(sum == known_sum)
		printf("TEST PASSED!\n");
	else
		printf("TEST FAILED!\n");
		
	return 0;
}
