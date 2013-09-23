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

extern "C" __global__ void __accrg_iter_matmul_1_1(
  double * A,
  double * B,
  double * C,
  int n)
{
  
  int i;
  int j;
  int __acc_tmp_0;
  int k;
  double c;
  
  i = __nv50_blockIdx_y;
  j = __nv50_blockIdx_x * __nv50_blockdim_x;
  j = __nv50_threadIdx_x + j;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(n > i)
  {
    j = __nv50_blockIdx_x * __nv50_blockdim_x;
    j = __nv50_threadIdx_x + j;
    while(n > j)
    {
      c = 0.0;
      k = 0;
      while(n > k)
      {
        c = c + (*(A + (unsigned int)((k + (n * i)))) ** (B + (unsigned int)((j + (n * k)))));
        k = k + 1;
      }
      * (C + (unsigned int)((j + (n * i)))) = c;
      j = j + __acc_tmp_0;
    }
    i = __nv50_griddim_y + i;
  }
} /* __accrg_iter_matmul_1_1 */

