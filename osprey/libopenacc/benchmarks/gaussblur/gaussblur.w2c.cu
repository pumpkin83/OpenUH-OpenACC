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

extern "C" __global__ void __accrg_gaussblur_1_1(
  int ny,
  int nx,
  double f,
  double * w0,
  const double s0,
  const double s1,
  const double s2,
  const double s4,
  const double s5,
  const double s8,
  double * w1)
{
  
  unsigned int j;
  unsigned int i;
  unsigned int __acc_tmp_0;
  
  j = __nv50_blockIdx_x * __nv50_blockdim_y;
  j = __nv50_threadIdx_y + j;
  __acc_tmp_0 = __nv50_blockdim_y * __nv50_griddim_x;
  while((int) j < (ny + -2))
  {
    i = __nv50_threadIdx_x;
    while((int) i < (nx + -2))
    {
      if((j >= (unsigned int)(2)) && (i >= (unsigned int)(2)))
      {
        * (w1 + (long long)((int) i + (nx * (int) j))) = f * ((((((*(w0 + (long long)((int) i + (nx * (int) j))) * s0) + (s1 * (*(w0 + (long long)((int) i + (nx * ((int) j + 1)))) + (*(w0 + (long long)((int) i + (nx * ((int) j + -1)))) + (*(w0 + (long long)(((int) i + (nx * (int) j)) + -1)) + *(w0 + (long long)(((int) i + (nx * (int) j)) + 1))))))) + (s2 * (*(w0 + (long long)(((int) i + (nx * ((int) j + 1))) + 1)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + 1))) + -1)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + -1))) + -1)) + *(w0 + (long long)(((int) i + (nx * ((int) j + -1))) + 1))))))) + (s4 * (*(w0 + (long long)((int) i + (nx * ((int) j + 2)))) + (*(w0 + (long long)((int) i + (nx * ((int) j + -2)))) + (*(w0 + (long long)(((int) i + (nx * (int) j)) + -2)) + *(w0 + (long long)(((int) i + (nx * (int) j)) + 2))))))) + (s5 * (*(w0 + (long long)(((int) i + (nx * ((int) j + 1))) + 2)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + 2))) + 1)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + 2))) + -1)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + 1))) + -2)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + -1))) + 2)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + -2))) + 1)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + -2))) + -1)) + *(w0 + (long long)(((int) i + (nx * ((int) j + -1))) + -2))))))))))) + (s8 * (*(w0 + (long long)(((int) i + (nx * ((int) j + 2))) + 2)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + 2))) + -2)) + (*(w0 + (long long)(((int) i + (nx * ((int) j + -2))) + -2)) + *(w0 + (long long)(((int) i + (nx * ((int) j + -2))) + 2)))))));
      }
      i = __nv50_blockdim_x + i;
    }
    j = j + __acc_tmp_0;
  }
} /* __accrg_gaussblur_1_1 */

