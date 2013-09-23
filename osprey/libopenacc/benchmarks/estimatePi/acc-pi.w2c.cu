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
  long long N,
  double * __reduction_pi)
{
  
  int i;
  double t;
  int __acc_tmp_0;
  int __acc_tmp_1;
  int __acc_tmp_2;
  
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  __acc_tmp_0 = __acc_tmp_0 * (__nv50_threadIdx_y + __nv50_blockIdx_y);
  __acc_tmp_1 = __nv50_threadIdx_x + (__nv50_blockIdx_x * __nv50_blockdim_x);
  __acc_tmp_1 = __acc_tmp_0 + __acc_tmp_1;
  * (__reduction_pi + __acc_tmp_1) = 0.0;
  i = __nv50_blockIdx_x * __nv50_blockdim_x;
  i = __nv50_threadIdx_x + i;
  __acc_tmp_2 = __nv50_blockdim_x * __nv50_griddim_x;
  while(N > (long long) i)
  {
    if(i >= (int)(0LL))
    {
      t = ((double)((long long) i) + 5.0e-01) / (double)(N);
      * (__reduction_pi + __acc_tmp_1) = *(__reduction_pi + __acc_tmp_1) + (4.0 / ((t * t) + 1.0));
    }
    i = __acc_tmp_2 + i;
  }
} /* __accrg_main_1_1 */

