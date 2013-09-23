#include <stdio.h>
#include <stdlib.h>
#include <openacc_rtl.h>

int main()
{
	double *host_A, *A;
	int i, N;
	double result;
	double gold_result;

	N = 2048;
	
	host_A = (double*)malloc(N*sizeof(double));

	srand((int)time(0));
	for(i=0; i<N; i++)
		host_A[i] = rand()/(RAND_MAX + 1.0);


	__accr_setup();
	printf("Initialization finished\n");
	
	__accr_malloc_on_device(host_A, (void**)&A, N*sizeof(double));
	__accr_memin_h2d(host_A, A, N*sizeof(double), 0);

	__accr_set_default_gang_vector();

	__accr_final_reduction_algorithm(&result, A, 0);
	printf("GPU reduction 0 result: %f\n", result);
/*	
	printf("GPU reduction 1 result: %f\n", result);

	result = __accr_final_reduction_algorithm(A, N, 2);
	printf("GPU reduction 2 result: %f\n", result);

	result = __accr_final_reduction_algorithm(A, N, 3);
	printf("GPU reduction 3 result: %f\n", result);
	
	result = __accr_final_reduction_algorithm(A, N, 4);
	printf("GPU reduction 4 result: %f\n", result);
	
	result = __accr_final_reduction_algorithm(A, N, 5);
	printf("GPU reduction 5 result: %f\n", result);
	
	result = __accr_final_reduction_algorithm(A, N, 6);
	printf("GPU reduction 6 result: %f\n", result);
*/

	__accr_memout_d2h(A, host_A, N*sizeof(double), 0);

	gold_result = 0;
	for(i=0; i<N; i++)
		gold_result += host_A[i];
	printf("CPU reduction result: %f\n", gold_result);
	

	__accr_cleanup();

}


