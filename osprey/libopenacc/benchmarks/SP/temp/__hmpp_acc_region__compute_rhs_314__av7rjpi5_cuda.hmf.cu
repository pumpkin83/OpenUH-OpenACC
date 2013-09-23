
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
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_314__av7rjpi5(hmpprt::s32 nx2_1, hmpprt::s32 nz2_1, double dssp_1, double* u_1, double* rhs, hmpprt::s32* __hmpp_addr__i, hmpprt::s32 j, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32* __hmpp_addr__m_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_314__av7rjpi5_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 nz2_2, double dssp_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32 j_2, hmpprt::s32* __hmpp_addr__k, hmpprt::s32* __hmpp_addr__m)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__compute_rhs_314__av7rjpi5_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__compute_rhs_314__av7rjpi5_parallel_region_1(double dssp, hmpprt::s32 j_1, hmpprt::s32* m, hmpprt::s32 nx2, hmpprt::s32 nz2, double* rhs_1, double* u);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__compute_rhs_314__av7rjpi5_parallel_region_1(double dssp, hmpprt::s32 j_1, hmpprt::s32* m, hmpprt::s32 nx2, hmpprt::s32 nz2, double* rhs_1, double* u)
{
 # 8 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 8 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 8 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 8 "<preprocessor>"
 iter_per_gang_1 = 1 + (nz2 - 1) / 192;
 # 8 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 8 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (nz2 - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (nz2 - 1));
 # 8 "<preprocessor>"
 hmpprt::s32 k_1;
 # 315 "rhs.c"
 # 315 "rhs.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 9 "<preprocessor>"
  hmpprt::s32 end_2;
  # 9 "<preprocessor>"
  hmpprt::s32 i_2;
  # 317 "rhs.c"
  # 317 "rhs.c"
  # 317 "rhs.c"
  for (i_2 = (hmpprt::gr_btidx()), end_2 = nx2 - 1 ; i_2 <= end_2 ; i_2 = i_2 + (hmpprt::gr_btnumx()))
  {
   # 7 "<preprocessor>"
   hmpprt::s32 m_1;
   # 318 "rhs.c"
   # 318 "rhs.c"
   for (m_1 = 0 ; m_1 <= 4 ; m_1 = m_1 + 1)
   {
    # 320 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) - dssp * (*(u + (i_2 + 1) + 37uLL * (j_1 - 2 + 37uLL * (k_1 + 1 + 36uLL * m_1))) - (double) 4.0 * *(u + (i_2 + 1) + 37uLL * (j_1 - 1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) + (double) 5.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1 + 36uLL * m_1))));
   }
   # 5 "<preprocessor>"
   # 5 "<preprocessor>"
   *m = 5;
  }
  # 5 "<preprocessor>"
 }
 # 5 "<preprocessor>"
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_314__av7rjpi5_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 nz2_2, double dssp_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32 j_2, hmpprt::s32* __hmpp_addr__k, hmpprt::s32* __hmpp_addr__m)
{
 # 7 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  m_2;
 # 7 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&m_2), hmpprt::MS_CUDA_GLOB, 4);
 # 7 "<preprocessor>"
 *m_2 = *__hmpp_addr__m;
 # 8 "<preprocessor>"
 hmpprt::s32 k;
 # 9 "<preprocessor>"
 hmpprt::s32 i_1;
 # 9 "<preprocessor>"
 i_1 = *__hmpp_addr__i_1;
 # 27 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter(&dssp_2, 8, "dssp");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (j_2), "j_1");
  __hmppcg_call.addLocalParameter(&m_2, 8, "m");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2_2), "nx2");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2_2), "nz2");
  __hmppcg_call.addLocalParameter(&rhs_2, 8, "rhs_1");
  __hmppcg_call.addLocalParameter(&u_2, 8, "u");
  __hmppcg_call.launch(__hmpp_acc_region__compute_rhs_314__av7rjpi5_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 27 "<preprocessor>"
 *__hmpp_addr__i_1 = i_1;
 # 28 "<preprocessor>"
 *__hmpp_addr__k = k;
 # 29 "<preprocessor>"
 *__hmpp_addr__m = *m_2;
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&m_2));
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_314__av7rjpi5(hmpprt::s32 nx2_1, hmpprt::s32 nz2_1, double dssp_1, double* u_1, double* rhs, hmpprt::s32* __hmpp_addr__i, hmpprt::s32 j, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32* __hmpp_addr__m_1)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__compute_rhs_314__av7rjpi5_internal_1(nx2_1, nz2_1, dssp_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (u_1), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs), __hmpp_addr__i, j, __hmpp_addr__k_1, __hmpp_addr__m_1));
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
      __hmpp_acc_region__compute_rhs_314__av7rjpi5_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__compute_rhs_314__av7rjpi5_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__compute_rhs_314__av7rjpi5", "prototype __hmpp_acc_region__compute_rhs_314__av7rjpi5(nx2: s32, nz2: s32, dssp: double, u: ^cudaglob double, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, j: s32, __hmpp_addr__k: ^host s32, __hmpp_addr__m: ^host s32)");

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
      delete __hmpp_acc_region__compute_rhs_314__av7rjpi5_parallel_region_1;

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
