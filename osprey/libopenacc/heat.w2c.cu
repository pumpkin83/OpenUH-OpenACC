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

extern "C" __global__ void __accrg_step_kernel_cpu_1_1(
  double * temp_in,
  double * temp_out,
  int ni,
  int nj,
  double tfac)
{
  
  int j;
  int i;
  int i00;
  int im10;
  int ip10;
  int i0m1;
  int i0p1;
  double d2tdx2;
  double d2tdy2;
  int __acc_tmp_0;
  
  j = __nv50_blockIdx_y;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(nj > j)
  {
    i = __nv50_blockIdx_x * __nv50_blockdim_x;
    i = __nv50_threadIdx_x + i;
    while(ni > i)
    {
      i00 = i + (ni * j);
      im10 = (i + (ni * j)) + -1;
      ip10 = (i + (ni * j)) + 1;
      i0m1 = i + (ni * (j + -1));
      i0p1 = i + (ni * (j + 1));
      d2tdx2 = *(temp_in + (unsigned int) ip10) + (*(temp_in + (unsigned int) im10) + (*(temp_in + (unsigned int) i00) * -2.0));
      d2tdy2 = *(temp_in + (unsigned int) i0p1) + (*(temp_in + (unsigned int) i0m1) + (*(temp_in + (unsigned int) i00) * -2.0));
      * (temp_out + (unsigned int) i00) = *(temp_in + (unsigned int) i00) + (tfac * (d2tdx2 + d2tdy2));
      i = i + __acc_tmp_0;
    }
    j = __nv50_griddim_y + j;
  }
} /* __accrg_step_kernel_cpu_1_1 */

