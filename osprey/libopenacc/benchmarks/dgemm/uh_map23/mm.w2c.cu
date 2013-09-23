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
  int __d_n,
  double * __d_A,
  double * __d_B,
  double * __d_C)
{
  
  int i;
  double c;
  int j;
  int k;
  unsigned int __acc_tmp_0;
  
  i = __nv50_blockIdx_y;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(__d_n > i)
  {
    j = (int)(__nv50_blockIdx_x) * (int)(__nv50_blockdim_x);
    j = j + (int)(__nv50_threadIdx_x);
    while(__d_n > j)
    {
      if((i >= 0) && (j >= 0))
      {
        c = 0.0;
        k = 0;
        while(__d_n > k)
        {
          _514 :;
          c = c + (*(__d_A + (long long)(k + (__d_n * i))) ** (__d_B + (long long)(j + (__d_n * k))));
          k = k + 1;
          _258 :;
        }
        _770 :;
        * (__d_C + (long long)(j + (__d_n * i))) = c;
      }
      j = j + (int)(__acc_tmp_0);
    }
    i = i + (int)(__nv50_griddim_y);
  }
} /* __accrg_iter_matmul_1_1 */

