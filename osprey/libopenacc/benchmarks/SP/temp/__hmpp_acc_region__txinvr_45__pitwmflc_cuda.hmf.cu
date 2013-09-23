
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
extern "C" CDLT_API  void __hmpp_acc_region__txinvr_45__pitwmflc()
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__txinvr_45__pitwmflc_internal_1()
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__txinvr_45__pitwmflc_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__txinvr_45__pitwmflc_parallel_region_1(hmpprt::s32 nx2_1, hmpprt::s32 ny2, hmpprt::s32 nz2_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__txinvr_45__pitwmflc_parallel_region_1(hmpprt::s32 nx2_1, hmpprt::s32 ny2, hmpprt::s32 nz2_1)
{
 # 21 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 21 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 21 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 21 "<preprocessor>"
 iter_per_gang_1 = 1 + (nz2_1 - 1) / 192;
 # 21 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 21 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (nz2_1 - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (nz2_1 - 1));
 # 21 "<preprocessor>"
 hmpprt::s32 k_1;
 # 47 "txinvr.c"
 # 47 "txinvr.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 22 "<preprocessor>"
  hmpprt::s32 end_2;
  # 22 "<preprocessor>"
  hmpprt::s32 j_1;
  # 48 "txinvr.c"
  # 48 "txinvr.c"
  # 48 "txinvr.c"
  for (j_1 = (hmpprt::gr_btidy()), end_2 = ny2 - 1 ; j_1 <= end_2 ; j_1 = j_1 + (hmpprt::gr_btnumy()))
  {
   # 23 "<preprocessor>"
   hmpprt::s32 end_1;
   # 23 "<preprocessor>"
   hmpprt::s32 i_2;
   # 49 "txinvr.c"
   # 49 "txinvr.c"
   # 49 "txinvr.c"
   for (i_2 = (hmpprt::gr_btidx()), end_1 = nx2_1 - 1 ; i_2 <= end_1 ; i_2 = i_2 + (hmpprt::gr_btnumx()))
   {
    }
   # 5 "<preprocessor>"
  }
  # 5 "<preprocessor>"
 }
 # 5 "<preprocessor>"
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__txinvr_45__pitwmflc_internal_1()
{
 # 33 "<preprocessor>"
 hmpprt::s32 nz2;
 # 34 "<preprocessor>"
 hmpprt::s32 ny2_1;
 # 35 "<preprocessor>"
 hmpprt::s32 nx2;
 # 5 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2), "nx2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2_1), "ny2");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2), "nz2_1");
  __hmppcg_call.launch(__hmpp_acc_region__txinvr_45__pitwmflc_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__txinvr_45__pitwmflc()
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__txinvr_45__pitwmflc_internal_1());
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
      __hmpp_acc_region__txinvr_45__pitwmflc_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__txinvr_45__pitwmflc_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__txinvr_45__pitwmflc", "prototype __hmpp_acc_region__txinvr_45__pitwmflc()");

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
      delete __hmpp_acc_region__txinvr_45__pitwmflc_parallel_region_1;

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
