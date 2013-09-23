
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
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_378__kqzzp2fd(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, double dssp_2, double* u_2, double* rhs_1, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32 k_2, hmpprt::s32* __hmpp_addr__m)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_378__kqzzp2fd_internal_1(hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, double dssp, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32 k, hmpprt::s32* __hmpp_addr__m_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__compute_rhs_378__kqzzp2fd_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__compute_rhs_378__kqzzp2fd_parallel_region_1(double dssp_1, hmpprt::s32* i_1, hmpprt::s32 k_1, hmpprt::s32* m, hmpprt::s32 nx2, hmpprt::s32 ny2, double* rhs, double* u);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__compute_rhs_378__kqzzp2fd_parallel_region_1(double dssp_1, hmpprt::s32* i_1, hmpprt::s32 k_1, hmpprt::s32* m, hmpprt::s32 nx2, hmpprt::s32 ny2, double* rhs, double* u)
{
 # 8 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 8 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 8 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 8 "<preprocessor>"
 iter_per_gang_1 = ((1 + (ny2 - 1) / 192) > 8LL ? (1 + (ny2 - 1) / 192) : 8LL);
 # 8 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 8 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (ny2 - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (ny2 - 1));
 # 8 "<preprocessor>"
 hmpprt::s32 j_1;
 # 379 "rhs.c"
 # 379 "rhs.c"
 for (j_1 = first_gang_iter_1 + (hmpprt::gr_btidy()) ; j_1 <= last_gang_iter_1 ; j_1 = j_1 + (hmpprt::gr_btnumy()))
 {
  # 5 "<preprocessor>"
  if (1 <= nx2)
  {
   # 9 "<preprocessor>"
   hmpprt::s32 end_2;
   # 9 "<preprocessor>"
   hmpprt::s32 i_2;
   # 380 "rhs.c"
   # 380 "rhs.c"
   # 380 "rhs.c"
   for (i_2 = 0, end_2 = nx2 - 1 ; i_2 <= end_2 ; i_2 = i_2 + 1)
   {
    # 7 "<preprocessor>"
    hmpprt::s32 m_1;
    # 381 "rhs.c"
    # 381 "rhs.c"
    for (m_1 = (hmpprt::gr_btidx()) ; m_1 <= 4 ; m_1 = m_1 + (hmpprt::gr_btnumx()))
    {
     # 383 "rhs.c"
     *(rhs + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 36uLL * m_1))) = *(rhs + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 36uLL * m_1))) - dssp_1 * ((double) 5.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 36uLL * m_1))) - (double) 4.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2 + 36uLL * m_1))));
    }
    # 5 "<preprocessor>"
    # 5 "<preprocessor>"
    *m = 5;
   }
   # 5 "<preprocessor>"
   # 5 "<preprocessor>"
   *i_1 = nx2 + 1;
  }
  else
  {
   # 5 "<preprocessor>"
   *i_1 = 1;
  }
 }
 # 5 "<preprocessor>"
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_378__kqzzp2fd_internal_1(hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, double dssp, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32 k, hmpprt::s32* __hmpp_addr__m_1)
{
 # 7 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  m_2;
 # 7 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&m_2), hmpprt::MS_CUDA_GLOB, 4);
 # 7 "<preprocessor>"
 *m_2 = *__hmpp_addr__m_1;
 # 8 "<preprocessor>"
 hmpprt::s32 j;
 # 9 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  i_3;
 # 9 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&i_3), hmpprt::MS_CUDA_GLOB, 4);
 # 9 "<preprocessor>"
 *i_3 = *__hmpp_addr__i_1;
 # 23 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter(&dssp, 8, "dssp_1");
  __hmppcg_call.addLocalParameter(&i_3, 8, "i_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (k), "k_1");
  __hmppcg_call.addLocalParameter(&m_2, 8, "m");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2_1), "nx2");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2_1), "ny2");
  __hmppcg_call.addLocalParameter(&rhs_2, 8, "rhs");
  __hmppcg_call.addLocalParameter(&u_1, 8, "u");
  __hmppcg_call.launch(__hmpp_acc_region__compute_rhs_378__kqzzp2fd_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 23 "<preprocessor>"
 *__hmpp_addr__i_1 = *i_3;
 # 24 "<preprocessor>"
 *__hmpp_addr__j_1 = j;
 # 25 "<preprocessor>"
 *__hmpp_addr__m_1 = *m_2;
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&m_2));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&i_3));
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_378__kqzzp2fd(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, double dssp_2, double* u_2, double* rhs_1, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32 k_2, hmpprt::s32* __hmpp_addr__m)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__compute_rhs_378__kqzzp2fd_internal_1(nx2_2, ny2_2, dssp_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (u_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs_1), __hmpp_addr__i, __hmpp_addr__j, k_2, __hmpp_addr__m));
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
      __hmpp_acc_region__compute_rhs_378__kqzzp2fd_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__compute_rhs_378__kqzzp2fd_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__compute_rhs_378__kqzzp2fd", "prototype __hmpp_acc_region__compute_rhs_378__kqzzp2fd(nx2: s32, ny2: s32, dssp: double, u: ^cudaglob double, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, k: s32, __hmpp_addr__m: ^host s32)");

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
      delete __hmpp_acc_region__compute_rhs_378__kqzzp2fd_parallel_region_1;

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
