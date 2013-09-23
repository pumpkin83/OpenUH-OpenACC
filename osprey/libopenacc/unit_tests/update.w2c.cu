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
  double * X,
  long long N)
{
  
  int i;
  int __acc_tmp_0;
  
  i = __nv50_blockIdx_x * __nv50_blockdim_x;
  i = __nv50_threadIdx_x + i;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(N > (long long) i)
  {
    if(i >= (int)(0LL))
    {
      //(*X0)[(long long) i] = (*X0)[(long long) i] * 2.0;
      *(X+(long long) i) = *(X +(long long) i) * 2.0;
    }
    i = __acc_tmp_0 + i;
  }
} /* __accrg_main_1_1 */

