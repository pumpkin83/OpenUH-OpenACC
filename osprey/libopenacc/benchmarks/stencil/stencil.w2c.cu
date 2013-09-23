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

extern "C" __global__ void __accrg_cpu_stencil_1_1(
  const int nx,
  const int ny,
  const int nz,
  float c1,
  float * A0,
  float c0,
  float * Anext)
{
  
  unsigned int i;
  unsigned int j;
  unsigned int k;
  unsigned int __acc_tmp_0;
  unsigned int __acc_tmp_1;
  
  i = __nv50_threadIdx_z;
  __acc_tmp_0 = __nv50_blockdim_y * __nv50_griddim_y;
  __acc_tmp_1 = __nv50_blockdim_x * __nv50_griddim_x;
  while((int) i < (nx + -1))
  {
    j = __nv50_blockIdx_y * __nv50_blockdim_y;
    j = __nv50_threadIdx_y + j;
    while((int) j < (ny + -1))
    {
      k = __nv50_blockIdx_x * __nv50_blockdim_x;
      k = __nv50_threadIdx_x + k;
      while((int) k < (nz + -1))
      {
        if((k >= (unsigned int)(1)) && ((i >= (unsigned int)(1)) && (j >= (unsigned int)(1))))
        {
          * (Anext + (long long)((int) i + (nx * ((int) j + (ny * (int) k))))) = (c1 * (*(A0 + (long long)(((int) i + (nx * ((int) j + (ny * (int) k)))) + -1)) + (*(A0 + (long long)(((int) i + (nx * ((int) j + (ny * (int) k)))) + 1)) + (*(A0 + (long long)((int) i + (nx * (((int) j + (ny * (int) k)) + -1)))) + (*(A0 + (long long)((int) i + (nx * (((int) j + (ny * (int) k)) + 1)))) + (*(A0 + (long long)((int) i + (nx * ((int) j + (ny * ((int) k + -1)))))) + *(A0 + (long long)((int) i + (nx * ((int) j + (ny * ((int) k + 1)))))))))))) - (*(A0 + (long long)((int) i + (nx * ((int) j + (ny * (int) k))))) * c0);
        }
        k = k + __acc_tmp_1;
      }
      j = j + __acc_tmp_0;
    }
    i = __nv50_blockdim_z + i;
  }
} /* __accrg_cpu_stencil_1_1 */

