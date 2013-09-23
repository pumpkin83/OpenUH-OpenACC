
#define __nv50_blockIdx_x blockIdx.x
#define __nv50_blockdim_x blockDim.x
#define __nv50_threadIdx_x threadIdx.x
#define __nv50_griddim_x gridDim.x

/*
__global__ void __accrg_main_1_1(a, b, c, n)
  double * a;
  double * b;
  double * c;
  int n;
*/
extern "C" __global__ void __accrg_main_1_1(double* a, double* b, double* c, int n)
{
  
  int i;
  int __acc_tmp_0;
  
  i = __nv50_blockIdx_x * __nv50_blockdim_x;
  i = __nv50_threadIdx_x + i;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(n > i)
  {
    * (c + (unsigned int) i) = *(a + (unsigned int) i) + *(b + (unsigned int) i);
    i = i + __acc_tmp_0;
  }
} /* __accrg_main_1_1 */
