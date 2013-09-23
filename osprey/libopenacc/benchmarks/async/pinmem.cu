#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void vectorAddGPU(float* a, float* b, float* c, int N)
{
  int idx;
  idx = blockIdx.x*blockDim.x + threadIdx.x;
  if (idx < N)
    c[idx] = a[idx] + b[idx];
}

#define MEMORY_ALIGNMENT  4096
#define ALIGN_UP(x,size) ( ((size_t)x+(size-1))&(~(size-1)) )

int main()
{
	int n, nelem;
	size_t bytes;
	// Pinned memory allocation on the CPU
	float *a, *b, *c;
	// Non-4K Aligned Pinned memory on the CPU
	float *a_UA, *b_UA, *c_UA; 

	// Device pointers for mapped memory
	float *d_a, *d_b, *d_c;
  
  	float errorNorm, refNorm, ref, diff;

	cudaSetDevice(0);

	/* Allocate mapped CPU memory */
	//nelem = 1048576;
	nelem = 1048575;
	bytes = nelem*sizeof(float);

	/*
    a_UA = (float *) malloc( bytes + MEMORY_ALIGNMENT );
    b_UA = (float *) malloc( bytes + MEMORY_ALIGNMENT );
    c_UA = (float *) malloc( bytes + MEMORY_ALIGNMENT );
	*/
    a_UA = (float *) malloc( bytes );
    b_UA = (float *) malloc( bytes );
    c_UA = (float *) malloc( bytes );
    
	for(n = 0; n < nelem; n++)
    {
    	a_UA[n] = rand() / (float)RAND_MAX;
    	b_UA[n] = rand() / (float)RAND_MAX;
  	}
    
	// We need to ensure memory is aligned to 4K (so we will need to padd memory accordingly)
    a = (float *) ALIGN_UP( a_UA, MEMORY_ALIGNMENT );
    b = (float *) ALIGN_UP( b_UA, MEMORY_ALIGNMENT );
    c = (float *) ALIGN_UP( c_UA, MEMORY_ALIGNMENT );
    
	cudaHostRegister(a, bytes, CU_MEMHOSTALLOC_DEVICEMAP);
    cudaHostRegister(b, bytes, CU_MEMHOSTALLOC_DEVICEMAP);
    cudaHostRegister(c, bytes, CU_MEMHOSTALLOC_DEVICEMAP);
 /* 
    for(n = 0; n < nelem; n++)
    {
    	a[n] = rand() / (float)RAND_MAX;
    	b[n] = rand() / (float)RAND_MAX;
  	}
 */
  	cudaHostGetDevicePointer((void **)&d_a, (void *)a, 0);
  	cudaHostGetDevicePointer((void **)&d_b, (void *)b, 0);
  	cudaHostGetDevicePointer((void **)&d_c, (void *)c, 0);
  
  	/* Call the GPU kernel using the CPU pointers residing in CPU mapped memory. */ 
  	dim3 block(256);
  	dim3 grid((unsigned int)ceil(nelem/(float)block.x));
  
  	vectorAddGPU<<<grid, block>>>(d_a, d_b, d_c, nelem);  
  	cudaDeviceSynchronize();

  	errorNorm = 0.f;
  	refNorm = 0.f;
  	for(n = 0; n < nelem; n++)
  	{
    	ref = a[n] + b[n];
    	diff = c[n] - ref;
    	errorNorm += diff*diff;
    	refNorm += ref*ref;
  	}
 	
	errorNorm = (float)sqrt((double)errorNorm);
  	refNorm = (float)sqrt((double)refNorm);

	printf("Error norm: %f\n", errorNorm);
	printf("Ref norm: %f\n", refNorm);

  	/* Memory clean up */
    cudaHostUnregister(a);
    cudaHostUnregister(b);
    cudaHostUnregister(c);
    free(a_UA);
    free(b_UA);
    free(c_UA);
}
