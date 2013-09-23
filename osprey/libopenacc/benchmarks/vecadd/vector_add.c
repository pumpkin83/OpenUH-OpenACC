#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

int main()
{
	int i;
	int *A, *B, *C, N;
	int *dev_A, *dev_B, *dev_C;
	unsigned int size;
	FILE *fp;
	int file_size;
	char *ptx_source;
	CUmodule cu_module;
	CUfunction cu_function;
	int result;

	N = 1024;
	A = (int*)malloc(N*sizeof(int));
	B = (int*)malloc(N*sizeof(int));
	C = (int*)malloc(N*sizeof(int));

	for(i=0; i<N; i++)
	{
		A[i] = i;
		B[i] = i;
		C[i] = 0;
	}

	size = N*sizeof(int);
	cudaMalloc((void**)&dev_A, size);
	cudaMalloc((void**)&dev_B, size);
	cudaMalloc((void**)&dev_C, size);

	cudaMemcpy(dev_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(dev_B, B, size, cudaMemcpyHostToDevice);
	
	fp = fopen("vector_add.ptx", "rb");
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	ptx_source = (char*)malloc((file_size+1)*sizeof(char));
	fseek(fp, 0, SEEK_SET);
	fread(ptx_source, sizeof(char), file_size, fp);
	fclose(fp);
	ptx_source[file_size] = '\0';

	//printf("Loading module %s\n", ptx_source);
	
	cuModuleLoadData(&cu_module, ptx_source);

	cuModuleGetFunction(&cu_function, cu_module, "vector_add");
	void *args[] = {&dev_A, &dev_B, &dev_C, &N};

	cuLaunchKernel(cu_function, 8, 1, 1, 
				   256, 1, 1,
				   0,
				   NULL, args, NULL);

	cudaMemcpy(C, dev_C, size, cudaMemcpyDeviceToHost);

	result = 0;
	for(i=0; i<N; i++)
	{
		if(C[i] != A[i] + B[i])
			result++;
	}
	
	if(result == 0)
		printf("Test successful\n");
	else
		printf("Test failed\n");

	free(A); free(B); free(C);
	cudaFree(dev_A);
	cudaFree(dev_B);
	cudaFree(dev_C);
	
}
