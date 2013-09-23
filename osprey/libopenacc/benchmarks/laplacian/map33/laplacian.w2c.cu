#define __nv50_blockIdx_x blockIdx.x
#define __nv50_blockIdx_y blockIdx.y
#define __nv50_blockIdx_z blockIdx.z
#define __nv50_threadIdx_x threadIdx.x
#define __nv50_threadIdx_y threadIdx.y
#define __nv50_threadIdx_z threadIdx.z
#define __nv50_blockdim_x blockDim.x
#define __nv50_blockdim_y blockDim.y
#define __nv50_blockdim_z blockDim.z
#define __nv50_griddim_x gridDim.x
#define __nv50_griddim_y gridDim.y
#define __nv50_griddim_z gridDim.z

extern "C" __global__ void __accrg_laplacian_1_1(
  int ns,
  int ny,
  int nx,
  double * w0,
  const double alpha,
  const double beta,
  double * w1)
{
  
  int k;
  int j;
  int i;
  unsigned int __acc_tmp_0;
  
  k = __nv50_threadIdx_x;
  __acc_tmp_0 = __nv50_blockdim_y * __nv50_griddim_y;
  while(k < (ns + -1))
  {
    j = (int)(__nv50_blockIdx_y) * (int)(__nv50_blockdim_y);
    j = j + (int)(__nv50_threadIdx_y);
    while(j < (ny + -1))
    {
      i = __nv50_blockIdx_x;
      while(i < (nx + -1))
      {
        if((i >= 1) && ((k >= 1) && (j >= 1)))
        {
          * (w1 + (long long)(i + ((nx * j) + (k * (ny * nx))))) = (*(w0 + (long long)(i + ((nx * j) + (k * (ny * nx))))) * alpha) + (beta * (*(w0 + (long long)(i + ((nx * j) + ((k + -1) * (ny * nx))))) + (*(w0 + (long long)(i + ((nx * j) + ((k + 1) * (ny * nx))))) + (*(w0 + (long long)(i + ((nx * (j + -1)) + (k * (ny * nx))))) + (*(w0 + (long long)(i + ((nx * (j + 1)) + (k * (ny * nx))))) + (*(w0 + (long long)((i + ((nx * j) + (k * (ny * nx)))) + -1)) + *(w0 + (long long)((i + ((nx * j) + (k * (ny * nx)))) + 1))))))));
        }
        i = i + (int)(__nv50_griddim_x);
      }
      j = j + (int)(__acc_tmp_0);
    }
    k = k + (int)(__nv50_blockdim_x);
  }
} /* __accrg_laplacian_1_1 */

