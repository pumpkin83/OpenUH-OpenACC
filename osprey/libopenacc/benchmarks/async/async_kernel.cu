extern "C" __global__ void async_kernel(float* a, float* b, float* c, int n)
{
	int ix, iy;
	int k;
	ix = blockIdx.x*blockDim.x + threadIdx.x;
	iy = blockIdx.y*blockDim.y + threadIdx.y;

	if(ix >= 0 && ix < n && iy >=0 && iy < n)
	{
		float tmp = 0.0f;
		for(k=0; k<n; k++)
		{
			tmp += a[iy*n + k]*b[k*n + ix];
		}
		c[iy*n + ix] = tmp;
	}
}
