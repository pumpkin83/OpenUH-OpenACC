
extern "C" __global__ void vector_addition(double* A, double* B, double* C, int N)
{
	int tx = threadIdx.x + blockIdx.x * blockDim.x;
	if(tx < N)
		C[tx] = A[tx] + B[tx];
}
