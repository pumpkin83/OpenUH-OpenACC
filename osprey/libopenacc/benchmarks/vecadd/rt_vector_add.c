#include <stdio.h>
#include <stdlib.h>
#include <openacc_rtl.h>

int main()
{
	double *A, *B, *C;
	double *dev_A, *dev_B, *dev_C;
	int i, N;
	unsigned int size;
	int result;

	N = 1024;
	size = N*sizeof(double);

	A = (double*)malloc(size);
	B = (double*)malloc(size);
	C = (double*)malloc(size);

	srand((int)time(0));
	for(i=0; i<N; i++)
	{
		A[i] = rand() % 100;
		B[i] = rand() % 100;
		C[i] = 0;
	}

	
	__accr_setup();

	__accr_malloc_on_device(A, (void**)&dev_A, size);
	__accr_malloc_on_device(B, (void**)&dev_B, size);
	__accr_malloc_on_device(C, (void**)&dev_C, size);
	
	printf("A device_addr: %p\n", dev_A);
	printf("B device_addr: %p\n", dev_B);
	printf("C device_addr: %p\n", dev_C);
	printf("N device_addr: %p\n", &N);

	__accr_memin_h2d(A, dev_A, size, 0);
	__accr_memin_h2d(B, dev_B, size, 0);

	__accr_push_kernel_param_pointer((void**)&dev_A);
	__accr_push_kernel_param_pointer((void**)&dev_B);
	__accr_push_kernel_param_pointer((void**)&dev_C);
	__accr_push_kernel_param_int(&N);

	__accr_set_gangs(8, 1, 1);
	
	__accr_set_vectors(256, 1, 1);

	__accr_launchkernel("vector_addition", "vector_add.ptx");

	__accr_memout_d2h(dev_C, C, size, 0);
	__accr_cleanup();
	
	result = 0;
	for(i=0; i<N; i++)
	{
		if(C[i] != A[i] + B[i])
			result++;
	}
	
	if(result == 0)
		printf("Test successful\n");
	else
	{
		printf("Test failed\n");
	#if 0
		for(i=0; i<N; i++)
		{
			printf("C[%d]: %f, sum: %f, A[%d]: %f, B[%d]: %f\n", i, C[i], A[i]+B[i], i, A[i], i, B[i]);
		}
	#endif
	}
	
	free(A); free(B); free(C);

	return 1;
}
