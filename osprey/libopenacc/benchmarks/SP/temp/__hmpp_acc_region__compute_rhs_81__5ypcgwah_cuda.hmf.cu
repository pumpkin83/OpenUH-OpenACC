
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
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_81__5ypcgwah(hmpprt::s32* grid_points, double* rhs_1, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, hmpprt::s32* __hmpp_addr__m_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_81__5ypcgwah_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  grid_points_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32* __hmpp_addr__m)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__compute_rhs_81__5ypcgwah_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__compute_rhs_81__5ypcgwah_parallel_region_1(double* forcing, hmpprt::s32* grid_points_1, hmpprt::s32* m_2, double* rhs);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__compute_rhs_81__5ypcgwah_parallel_region_1(double* forcing, hmpprt::s32* grid_points_1, hmpprt::s32* m_2, double* rhs)
{
 # 8 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 8 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 8 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 8 "<preprocessor>"
 iter_per_gang_1 = 1 + (*(grid_points_1 + 2) - 1) / 192;
 # 8 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 8 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (*(grid_points_1 + 2) - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (*(grid_points_1 + 2) - 1));
 # 8 "<preprocessor>"
 hmpprt::s32 k_1;
 # 82 "rhs.c"
 # 82 "rhs.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 9 "<preprocessor>"
  hmpprt::s32 end_3;
  # 9 "<preprocessor>"
  hmpprt::s32 j_1;
  # 84 "rhs.c"
  # 84 "rhs.c"
  # 84 "rhs.c"
  for (j_1 = (hmpprt::gr_btidx()), end_3 = *(grid_points_1 + 1) - 1 ; j_1 <= end_3 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
  {
   # 10 "<preprocessor>"
   hmpprt::s32 end_2;
   # 10 "<preprocessor>"
   hmpprt::s32 i_2;
   # 86 "rhs.c"
   # 86 "rhs.c"
   # 86 "rhs.c"
   for (i_2 = 0, end_2 = *grid_points_1 - 1 ; i_2 <= end_2 ; i_2 = i_2 + 1)
   {
    # 7 "<preprocessor>"
    hmpprt::s32 m_1;
    # 87 "rhs.c"
    # 87 "rhs.c"
    for (m_1 = 0 ; m_1 <= 4 ; m_1 = m_1 + 1)
    {
     # 88 "rhs.c"
     *(rhs + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 36uLL * m_1))) = *(forcing + i_2 + 37uLL * (k_1 + 36uLL * (m_1 + 5uLL * j_1)));
    }
    # 5 "<preprocessor>"
    # 5 "<preprocessor>"
    *m_2 = 5;
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
void __hmpp_acc_region__compute_rhs_81__5ypcgwah_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  grid_points_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32* __hmpp_addr__m)
{
 # 7 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  m;
 # 7 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&m), hmpprt::MS_CUDA_GLOB, 4);
 # 7 "<preprocessor>"
 *m = *__hmpp_addr__m;
 # 8 "<preprocessor>"
 hmpprt::s32 k;
 # 9 "<preprocessor>"
 hmpprt::s32 j;
 # 9 "<preprocessor>"
 j = *__hmpp_addr__j_1;
 # 10 "<preprocessor>"
 hmpprt::s32 i_1;
 # 10 "<preprocessor>"
 i_1 = *__hmpp_addr__i;
 # 11 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  forcing_1;
 # 34 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&forcing_1), hmpprt::MS_CUDA_GLOB, 1971360uLL);
 # 34 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter(&forcing_1, 8, "forcing");
  __hmppcg_call.addLocalParameter(&grid_points_2, 8, "grid_points_1");
  __hmppcg_call.addLocalParameter(&m, 8, "m_2");
  __hmppcg_call.addLocalParameter(&rhs_2, 8, "rhs");
  __hmppcg_call.launch(__hmpp_acc_region__compute_rhs_81__5ypcgwah_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 34 "<preprocessor>"
 *__hmpp_addr__i = i_1;
 # 35 "<preprocessor>"
 *__hmpp_addr__j_1 = j;
 # 36 "<preprocessor>"
 *__hmpp_addr__k_1 = k;
 # 37 "<preprocessor>"
 *__hmpp_addr__m = *m;
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&m));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&forcing_1));
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_81__5ypcgwah(hmpprt::s32* grid_points, double* rhs_1, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, hmpprt::s32* __hmpp_addr__m_1)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__compute_rhs_81__5ypcgwah_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32> (grid_points), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs_1), __hmpp_addr__i_1, __hmpp_addr__j, __hmpp_addr__k, __hmpp_addr__m_1));
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
      __hmpp_acc_region__compute_rhs_81__5ypcgwah_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__compute_rhs_81__5ypcgwah_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__compute_rhs_81__5ypcgwah", "prototype __hmpp_acc_region__compute_rhs_81__5ypcgwah(grid_points: ^cudaglob s32, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32, __hmpp_addr__m: ^host s32)");

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
      delete __hmpp_acc_region__compute_rhs_81__5ypcgwah_parallel_region_1;

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
