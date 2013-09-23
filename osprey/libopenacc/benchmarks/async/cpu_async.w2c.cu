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
  int n1,
  float * a1,
  float * b1,
  float * c1)
{
  
  int i;
  int j;
  float tmp;
  int k;
  unsigned int __acc_tmp_0;
  unsigned int __acc_tmp_1;
  
  i = (int)(__nv50_blockIdx_y) * (int)(__nv50_blockdim_y);
  i = i + (int)(__nv50_threadIdx_y);
  __acc_tmp_0 = __nv50_blockdim_y * __nv50_griddim_y;
  __acc_tmp_1 = __nv50_blockdim_x * __nv50_griddim_x;
  while(n1 > i)
  {
    j = (int)(__nv50_blockIdx_x) * (int)(__nv50_blockdim_x);
    j = j + (int)(__nv50_threadIdx_x);
    while(n1 > j)
    {
      if((i >= 0) && (j >= 0))
      {
        tmp = 0.0F;
        k = 0;
        while(n1 > k)
        {
          _3586 :;
          tmp = tmp + (*(a1 + (long long)(k + (n1 * i))) ** (b1 + (long long)(j + (n1 * k))));
          k = k + 1;
          _3330 :;
        }
        _3842 :;
        * (c1 + (long long)(j + (n1 * i))) = tmp;
      }
      j = j + (int)(__acc_tmp_1);
    }
    i = i + (int)(__acc_tmp_0);
  }
} /* __accrg_main_1_1 */

