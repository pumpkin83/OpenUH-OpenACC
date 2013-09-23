
#include <stdio.h>

#ifndef __CUDACC__
#include <stdlib.h>
#include <math.h>

#include <hmpprt/Grouplet.h>
#include <hmpprt/HostTypes.h>
#include <hmpprt/Context.h>
#include <hmpprt/CUDAGrid.h>
#include <hmpprt/CUDAModule.h>
#include <hmpprt/DeviceManager.h>
#include <hmpperr/hmpperr.h>

#ifdef _WIN32
#  define CDLT_API __declspec(dllexport)
#else /* ! _WIN32 */
#  define CDLT_API
#endif /* _WIN32 */



#else // ! __CUDACC__

#include <hmpprt/HostTypes.h>
#include <hmpprt/CUDAIntrinsics.h>

extern __shared__ int64_t hmpp_sharedmem[];
#endif // __CUDACC__



#ifndef __CUDACC__

#else


#endif

#define HMPPCG_SIMD_LENGTH 32

# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__iter_matmul_60__16hcgydq(double* A_1, double* B_1, double* C_1, hmpprt::s32 n_11)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__iter_matmul_60__16hcgydq_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  A_11, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  B_11, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  C_11, hmpprt::s32 n_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__iter_matmul_60__16hcgydq_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__iter_matmul_60__16hcgydq_parallel_region_1(double* A, double* B, double* C, hmpprt::s32 n);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__iter_matmul_60__16hcgydq_parallel_region_1(double* A, double* B, double* C, hmpprt::s32 n)
{
 # 9 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 9 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 9 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 9 "<preprocessor>"
 iter_per_gang_1 = 1 + (n - 1) / 192;
 # 9 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 9 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (n - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (n - 1));
 # 9 "<preprocessor>"
 hmpprt::s32 i_1;
 # 64 "acc_mm.c"
 # 65 "acc_mm.c"
 for (i_1 = first_gang_iter_1 ; i_1 <= last_gang_iter_1 ; i_1 = i_1 + 1)
 {
  # 8 "<preprocessor>"
  hmpprt::s32 end_2;
  # 8 "<preprocessor>"
  hmpprt::s32 j_1;
  # 67 "acc_mm.c"
  # 68 "acc_mm.c"
  # 68 "acc_mm.c"
  for (j_1 = (hmpprt::gr_btidx()), end_2 = n - 1 ; j_1 <= end_2 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
  {
   # 69 "acc_mm.c"
   double c;
   # 69 "acc_mm.c"
   c = (double) 0.0;
   # 7 "<preprocessor>"
   hmpprt::s32 end_1;
   # 7 "<preprocessor>"
   hmpprt::s32 k_1;
   # 70 "acc_mm.c"
   # 70 "acc_mm.c"
   # 70 "acc_mm.c"
   for (k_1 = 0, end_1 = n - 1 ; k_1 <= end_1 ; k_1 = k_1 + 1)
   {
    # 71 "acc_mm.c"
    c = c + *(A + (i_1 * n + k_1)) * *(B + (k_1 * n + j_1));
   }
   # 72 "acc_mm.c"
   # 72 "acc_mm.c"
   *(C + (i_1 * n + j_1)) = c;
  }
  # 5 "<preprocessor>"
 }
 # 5 "<preprocessor>"
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__iter_matmul_60__16hcgydq_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  A_11, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  B_11, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  C_11, hmpprt::s32 n_1)
{
 # 5 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter(&A_11, 8, "A");
  __hmppcg_call.addLocalParameter(&B_11, 8, "B");
  __hmppcg_call.addLocalParameter(&C_11, 8, "C");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (n_1), "n");
  __hmppcg_call.launch(__hmpp_acc_region__iter_matmul_60__16hcgydq_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__iter_matmul_60__16hcgydq(double* A_1, double* B_1, double* C_1, hmpprt::s32 n_11)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__iter_matmul_60__16hcgydq_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (A_1), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (B_1), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (C_1), n_11));
}
#endif // __CUDACC__




#ifndef __CUDACC__
extern "C" const char * hmpprt_cuda_get_gpu_code();

static hmpprt::CUDAModule * hmpprt_module = 0;
static int hmpprt_uses = 0;

extern "C" CDLT_API void * hmpprt_init()
{
  try
  {
    if (hmpprt_uses++ == 0)
    {
      hmpprt_module = new hmpprt::CUDAModule(hmpprt_cuda_get_gpu_code());
      __hmpp_acc_region__iter_matmul_60__16hcgydq_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__iter_matmul_60__16hcgydq_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__iter_matmul_60__16hcgydq", "prototype __hmpp_acc_region__iter_matmul_60__16hcgydq(A: ^cudaglob double, B: ^cudaglob double, C: ^cudaglob double, n: s32)");

  }
  catch (hmpperr::Error & e)
  {
    return e.clone();
  }
  catch(...)
  {
    fprintf(stderr,"Unexpected error in hmpprt_init()\n");
    abort();
  }
  return 0;
}
#endif // __CUDACC__

#ifndef __CUDACC__
extern "C" CDLT_API void * hmpprt_fini()
{
  try
  {
    if (--hmpprt_uses == 0)
    {
      delete __hmpp_acc_region__iter_matmul_60__16hcgydq_parallel_region_1;

      delete hmpprt_module;
      hmpprt_module = 0;
    }
  }
  catch (hmpperr::Error & e)
  {
    return e.clone();
  }
  catch(...)
  {
    fprintf(stderr,"Unexpected error in hmpprt_fini()\n");
    abort();
  }
  return 0;
}
#endif // __CUDACC__

// footer
