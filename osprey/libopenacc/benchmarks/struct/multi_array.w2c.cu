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
  int * __d_x,
  int * __d_y)
{
  
  int k;
  int j;
  int i;
  unsigned int __acc_tmp_0;
  
  k = __nv50_blockIdx_x;
  __acc_tmp_0 = __nv50_blockdim_y * __nv50_griddim_y;
  while(k <= 7)
  {
    j = (int)(__nv50_blockIdx_y) * (int)(__nv50_blockdim_y);
    j = j + (int)(__nv50_threadIdx_y);
    while(j <= 15)
    {
      i = __nv50_threadIdx_x;
      while(i <= 127)
      {
        if((i >= 0) && ((k >= 0) && (j >= 0)))
        {
          * (__d_y + i + ((k * 2048LL) + (j * 128))) = (*(__d_x + i + ((k * 2048LL) + (j * 128))) * 3) + 2;
        }
        i = i + (int)(__nv50_blockdim_x);
      }
      j = j + (int)(__acc_tmp_0);
    }
    k = k + (int)(__nv50_griddim_x);
  }
} /* __accrg_main_1_1 */

