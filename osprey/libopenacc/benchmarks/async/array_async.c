#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>

CUdevice cu_device;
CUcontext cu_context;
CUmodule cu_module;
CUfunction cu_function;

float get_sum(float* array, int n)
{
	int i, j;
	float sum = 0.0f;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			sum += array[i*n + j];
	}
	return sum;
}


int main(int argc, char** argv)
{

	int n1, n2;
	int i, j, k;
	unsigned int dev_bytes, host_bytes;
	float *a1, *b1, *c1;
	float *dev_a, *dev_b, *dev_c;
	float *a2, *b2, *c2;
	struct timeval tim;
	double start, end;
	float device_sum, host_sum;
	
	n1 = 8192;
	n2 = 1024;
	
	cuInit(0);
	cuDeviceGet(&cu_device, 0);
	cuCtxCreate(&cu_context, 0, cu_device);
	
	srand(1234);
	dev_bytes = n1*n1*sizeof(float);
	a1 = (float*)valloc(dev_bytes);
	b1 = (float*)valloc(dev_bytes);
	c1 = (float*)valloc(dev_bytes);
	
	host_bytes = n2*n2*sizeof(float);
	a2 = (float*)malloc(host_bytes);
	b2 = (float*)malloc(host_bytes);
	c2 = (float*)malloc(host_bytes);
	
	cudaMalloc((void**)&dev_a, dev_bytes);
	cudaMalloc((void**)&dev_b, dev_bytes);
	cudaMalloc((void**)&dev_c, dev_bytes);
	
	for(i=0; i<n1; i++){
		for(j=0; j<n1; j++){
			a1[i*n1 + j] = rand()/(float)RAND_MAX;
			b1[i*n1 + j] = rand()/(float)RAND_MAX;
			c1[i*n1 + j] = 0.0f;
		}
	}
	
	for(i=0; i<n2; i++){
		for(j=0; j<n2; j++){
			a2[i*n2 + j] = rand()/(float)RAND_MAX;
			b2[i*n2 + j] = rand()/(float)RAND_MAX;
			c2[i*n2 + j] = 0.0f;
		}
	}
	
	gettimeofday(&tim, NULL);
	start = tim.tv_sec + (tim.tv_usec/1000000.0);
	
	cudaHostRegister(a1, dev_bytes, cudaHostRegisterMapped);
    cudaHostRegister(b1, dev_bytes, cudaHostRegisterMapped);
    cudaHostRegister(c1, dev_bytes, cudaHostRegisterMapped);
	
	cudaStream_t stream0[12]={NULL};
	cuStreamCreate(&stream0[9], 0);

	cudaMemcpyAsync(dev_a, a1, dev_bytes, cudaMemcpyHostToDevice, stream0[9]);
	cudaMemcpyAsync(dev_b, b1, dev_bytes, cudaMemcpyHostToDevice, stream0[9]);

	cuModuleLoad(&cu_module, "async_kernel.ptx");
	cuModuleGetFunction(&cu_function, cu_module, "async_kernel");
	void *args[] = {&dev_a, &dev_b, &dev_c, &n1};

	cuLaunchKernel(cu_function, 256, 256, 1, 
				   32, 32, 1,
				   0,
				   stream0[9], args, NULL);
	
	/* the compuation on the host */
	for(i=0; i<n2; i++){
		for(j=0; j<n2; j++){
			float tmp = 0.0f;
			for(k=0; k<n2; k++)
				tmp += a2[i*n2 + k]*b2[k*n2 + j];
			c2[i*n2 + j] = tmp;
		}
	}

	cudaMemcpyAsync(c1, dev_c, dev_bytes, cudaMemcpyDeviceToHost, stream0[9]);
	cuStreamSynchronize(stream0[9]);
	
	gettimeofday(&tim, NULL);
    end = tim.tv_sec + (tim.tv_usec/1000000.0);
    printf("Time for async computing: %.2f s\n", end - start);
	
	device_sum = get_sum(c1, n1);
	host_sum = get_sum(c2, n2);
	printf("Device sum: %f\n", device_sum);
	printf("Host sum: %f\n", host_sum);

	cudaHostUnregister(a1);
    cudaHostUnregister(b1);
    cudaHostUnregister(c1);

	cuStreamDestroy(stream0[9]);
	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_c);

	free(a1); free(b1); free(c1);
	free(a2); free(b2); free(c2);

	cuCtxDestroy(cu_context);
}
