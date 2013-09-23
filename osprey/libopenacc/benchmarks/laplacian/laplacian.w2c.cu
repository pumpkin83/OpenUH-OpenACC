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
  
  unsigned int k;
  unsigned int j;
  unsigned int i;
  unsigned int __acc_tmp_0;
  
  k = __nv50_threadIdx_x;
  __acc_tmp_0 = __nv50_blockdim_y * __nv50_griddim_y;
  while((int) k < (ns + -1))
  {
    j = __nv50_blockIdx_y * __nv50_blockdim_y;
    j = __nv50_threadIdx_y + j;
    while((int) j < (ny + -1))
    {
      i = __nv50_blockIdx_x;
      while((int) i < (nx + -1))
      {
        if((i >= (unsigned int)(1)) && ((k >= (unsigned int)(1)) && (j >= (unsigned int)(1))))
        {
          * (w1 + (long long)((int) i + ((nx * (int) j) + ((int) k * (ny * nx))))) = (*(w0 + (long long)((int) i + ((nx * (int) j) + ((int) k * (ny * nx))))) * alpha) + (beta * (*(w0 + (long long)((int) i + ((nx * (int) j) + (((int) k + -1) * (ny * nx))))) + (*(w0 + (long long)((int) i + ((nx * (int) j) + (((int) k + 1) * (ny * nx))))) + (*(w0 + (long long)((int) i + ((nx * ((int) j + -1)) + ((int) k * (ny * nx))))) + (*(w0 + (long long)((int) i + ((nx * ((int) j + 1)) + ((int) k * (ny * nx))))) + (*(w0 + (long long)(((int) i + ((nx * (int) j) + ((int) k * (ny * nx)))) + -1)) + *(w0 + (long long)(((int) i + ((nx * (int) j) + ((int) k * (ny * nx)))) + 1))))))));
        }
        i = __nv50_griddim_x + i;
      }
      j = j + __acc_tmp_0;
    }
    k = __nv50_blockdim_x + k;
  }
} /* __accrg_laplacian_1_1 */

