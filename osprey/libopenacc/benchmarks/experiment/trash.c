#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

int main()
{
	int i;
	int *a;
	if(a == NULL)
		printf("a is NULL\n");
	else
		printf("a is not NULL\n");

	cudaStream_t streams[12] = {NULL};
	printf("Before initilization\n");
	for(i=0; i<12; i++)
	{
		if(streams[i] == NULL)
			printf("stream %d is NULL\n", i);
		else
			printf("stream %d is not NULL\n", i);
			
	}

	printf("After initilization\n");
	for(i=0; i<12; i++)
	{
		cuStreamCreate(&streams[i], 0);
		if(streams[i] == NULL)
			printf("stream %d is NULL\n", i);
		else
			printf("stream %d is not NULL\n", i);
	}

	return 0;
}

