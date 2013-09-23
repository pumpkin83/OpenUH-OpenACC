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

extern "C" __global__ void __accrg_jacobi__1_1(
  int ny,
  int nx,
  double * w0,
  double c0,
  double c1,
  double c2,
  double * w1)
{
  
  int j;
  int i;
  int i00;
  int im10;
  int ip10;
  int i0m1;
  int i0p1;
  int im1m1;
  int im1p1;
  int ip1m1;
  int ip1p1;
  unsigned int __acc_tmp_0;
  
  j = __nv50_blockIdx_y;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(j < (ny + -1))
  {
    i = (int)(__nv50_blockIdx_x) * (int)(__nv50_blockdim_x);
    i = i + (int)(__nv50_threadIdx_x);
    while(i < (nx + -1))
    {
      if((j >= 1) && (i >= 1))
      {
        i00 = i + (nx * j);
        im10 = (i + (nx * j)) + -1;
        ip10 = (i + (nx * j)) + 1;
        i0m1 = (i + (nx * j)) + -1;
        i0p1 = (i + (nx * j)) + 1;
        im1m1 = (i + (nx * j)) + -2;
        im1p1 = i + (nx * j);
        ip1m1 = i + (nx * j);
        ip1p1 = (i + (nx * j)) + 2;
        * (w1 + (unsigned long long)((unsigned long long) i00)) = ((*(w0 + (unsigned long long)((unsigned long long) i00)) * c0) + (c1 * (*(w0 + (unsigned long long)((unsigned long long) i0p1)) + (*(w0 + (unsigned long long)((unsigned long long) ip10)) + (*(w0 + (unsigned long long)((unsigned long long) im10)) + *(w0 + (unsigned long long)((unsigned long long) i0m1))))))) + (c2 * (*(w0 + (unsigned long long)((unsigned long long) ip1p1)) + (*(w0 + (unsigned long long)((unsigned long long) ip1m1)) + (*(w0 + (unsigned long long)((unsigned long long) im1m1)) + *(w0 + (unsigned long long)((unsigned long long) im1p1))))));
      }
      i = i + (int)(__acc_tmp_0);
    }
    j = j + (int)(__nv50_griddim_y);
  }
} /* __accrg_jacobi__1_1 */

