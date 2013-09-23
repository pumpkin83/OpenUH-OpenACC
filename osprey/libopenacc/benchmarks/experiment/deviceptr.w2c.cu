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

extern "C" __global__ void __accrg_main_1_1(
  int N,
  int * A,
  int * B)
{
  
  unsigned int i;
  unsigned int __acc_tmp_0;
  
  i = __nv50_blockIdx_x * __nv50_blockdim_x;
  i = __nv50_threadIdx_x + i;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(N > (int) i)
  {
    if(1)
    {
      * (A + (unsigned long long)((unsigned long long) i)) = (int) i + 1;
      * (B + (unsigned long long)((unsigned long long) i)) = *(A + (unsigned long long)((unsigned long long) i));
    }
    i = i + __acc_tmp_0;
  }
} /* __accrg_main_1_1 */

