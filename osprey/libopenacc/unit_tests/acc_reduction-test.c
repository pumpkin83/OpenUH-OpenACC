#include <stdio.h>
#include <stdlib.h>
#include <openacc_rtl.h>

int main()
{
	double *A;
	int i, N;
	double result;
	double gold_result;

	N = 1020;
	
	A = (double*)malloc(N*sizeof(double));

	srand((int)time(0));
	for(i=0; i<N; i++)
		A[i] = rand()/(RAND_MAX + 1.0);

	__accr_setup();
	printf("Initialization finished\n");
	fflush(stdout);
	
	result = run_reduction(A, N, 0);
	printf("GPU reduction 0 result: %f\n", result);
	
	result = run_reduction(A, N, 1);
	printf("GPU reduction 1 result: %f\n", result);

	result = run_reduction(A, N, 2);
	printf("GPU reduction 2 result: %f\n", result);

	result = run_reduction(A, N, 3);
	printf("GPU reduction 3 result: %f\n", result);
	
	result = run_reduction(A, N, 4);
	printf("GPU reduction 4 result: %f\n", result);
	
	result = run_reduction(A, N, 5);
	printf("GPU reduction 5 result: %f\n", result);
	
	result = run_reduction(A, N, 6);
	printf("GPU reduction 6 result: %f\n", result);

	gold_result = 0;
	for(i=0; i<N; i++)
		gold_result += A[i];
	printf("CPU reduction result: %f\n", gold_result);
	

	__accr_cleanup();

}


