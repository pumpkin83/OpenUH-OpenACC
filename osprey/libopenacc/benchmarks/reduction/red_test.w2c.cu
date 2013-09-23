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

extern "C" __global__ void __accrg_reduction_ADD_int32_1_1(
  int * g_in_sum,
  int * g_out_sum,
  unsigned int n_of_sum,
  unsigned int blocksize,
  unsigned int is_power2)
{
  
  extern __shared__ int __sdata_sum[];
  unsigned int tid;
  unsigned int i;
  unsigned int gridSize;
  int mySum;
  volatile int * __smem_sum;
  
  tid = __nv50_threadIdx_x;
  i = (__nv50_threadIdx_x * (blocksize * __nv50_blockIdx_x)) * 2U;
  gridSize = (blocksize * __nv50_griddim_x) * 2U;
  mySum = 0U;
  while(n_of_sum > i)
  {
    mySum = *((int *)((unsigned int)(g_in_sum) + (i * 4U))) + mySum;
    if((is_power2 == 1U) || (n_of_sum > (blocksize + i)))
    {
      mySum = *((int *)((unsigned int)(g_in_sum) + ((blocksize + i) * 4U))) + mySum;
    }
    i = i + gridSize;
  }
  * ((int *)((unsigned int)(__sdata_sum) + (tid * 4U))) = mySum;
  __syncthreads();
  if(blocksize >= 512U)
  {
    if(tid < 256U)
    {
      mySum = *(((int *)((unsigned int)(__sdata_sum) + (tid * 4U))) + 256LL) + mySum;
      * ((int *)((unsigned int)(__sdata_sum) + (tid * 4U))) = mySum;
    }
    __syncthreads();
  }
  if(blocksize >= 256U)
  {
    if(tid < 128U)
    {
      mySum = *(((int *)((unsigned int)(__sdata_sum) + (tid * 4U))) + 128LL) + mySum;
      * ((int *)((unsigned int)(__sdata_sum) + (tid * 4U))) = mySum;
    }
    __syncthreads();
  }
  if(blocksize >= 128U)
  {
    if(tid < 64U)
    {
      mySum = *(((int *)((unsigned int)(__sdata_sum) + (tid * 4U))) + 64LL) + mySum;
      * ((int *)((unsigned int)(__sdata_sum) + (tid * 4U))) = mySum;
    }
    __syncthreads();
  }
  if(tid < 32U)
  {
    __smem_sum = __sdata_sum;
    if(blocksize >= 64U)
    {
      if(tid < 32U)
      {
        mySum = mySum + *(((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) + 32LL);
        * ((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) = mySum;
      }
      __syncthreads();
    }
    if(blocksize >= 32U)
    {
      if(tid < 16U)
      {
        mySum = mySum + *(((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) + 16LL);
        * ((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) = mySum;
      }
      __syncthreads();
    }
    if(blocksize >= 16U)
    {
      if(tid < 8U)
      {
        mySum = mySum + *(((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) + 8LL);
        * ((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) = mySum;
      }
      __syncthreads();
    }
    if(blocksize >= 8U)
    {
      if(tid < 4U)
      {
        mySum = mySum + *(((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) + 4LL);
        * ((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) = mySum;
      }
      __syncthreads();
    }
    if(blocksize >= 4U)
    {
      if(tid < 2U)
      {
        mySum = mySum + *(((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) + 2LL);
        * ((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) = mySum;
      }
      __syncthreads();
    }
    if(blocksize >= 2U)
    {
      if(tid < 1U)
      {
        mySum = mySum + *(((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) + 1LL);
        * ((volatile int *)((unsigned int)(__smem_sum) + (tid * 4U))) = mySum;
      }
      __syncthreads();
    }
  }
  if(tid == 0U)
  {
    * ((int *)((unsigned int)(g_out_sum) + (__nv50_blockIdx_x * 4U))) = *__sdata_sum;
  }
} /* __accrg_reduction_ADD_int32_1_1 */


extern "C" __global__ void __accrg_test1_1_2(
  int NJ,
  int NI,
  int * input,
  int sum,
  int * __reduction_sum)
{
  
  int j;
  int local_sum;
  int i;
  unsigned int __acc_tmp_0;
  unsigned int __acc_tmp_1;
  unsigned int __acc_tmp_2;
  
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  __acc_tmp_0 = __acc_tmp_0 * (__nv50_threadIdx_y + __nv50_blockIdx_y);
  __acc_tmp_1 = __nv50_threadIdx_x + (__nv50_blockIdx_x * __nv50_blockdim_x);
  __acc_tmp_1 = __acc_tmp_0 + __acc_tmp_1;
  * (__reduction_sum + __acc_tmp_1) = 0;
  j = __nv50_blockIdx_x * __nv50_blockdim_x;
  j = (unsigned int)(j) + __nv50_threadIdx_x;
  __acc_tmp_2 = __nv50_blockdim_x * __nv50_griddim_x;
  while(NJ > j)
  {
    if(j >= 0)
    {
      local_sum = 0;
      i = 0;
      while(NI > i)
      {
        _2050 :;
        local_sum = *(input + (long long)(i + (NI * j))) + local_sum;
        i = i + 1;
        _1794 :;
      }
      _2306 :;
      sum = sum + local_sum;
    }
    j = j + (int)(__acc_tmp_2);
  }
} /* __accrg_test1_1_2 */

