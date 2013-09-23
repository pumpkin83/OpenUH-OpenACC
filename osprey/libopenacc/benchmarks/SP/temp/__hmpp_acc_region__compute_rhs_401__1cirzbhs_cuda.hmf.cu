
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
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_401__1cirzbhs(hmpprt::s32 nx2_1, hmpprt::s32 ny2, hmpprt::s32 nz2_1, double dssp_1, double* u_2, double* rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, hmpprt::s32* __hmpp_addr__m)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_401__1cirzbhs_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, hmpprt::s32 nz2_2, double dssp_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_1, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32* __hmpp_addr__m_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__compute_rhs_401__1cirzbhs_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__compute_rhs_401__1cirzbhs_parallel_region_1(double dssp, hmpprt::s32* m, hmpprt::s32 nx2, hmpprt::s32 ny2_1, hmpprt::s32 nz2, double* rhs, double* u);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__compute_rhs_401__1cirzbhs_parallel_region_1(double dssp, hmpprt::s32* m, hmpprt::s32 nx2, hmpprt::s32 ny2_1, hmpprt::s32 nz2, double* rhs, double* u)
{
 # 8 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 8 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 8 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 8 "<preprocessor>"
 iter_per_gang_1 = 1 + (nz2 - 5) / 192;
 # 8 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 8 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (nz2 - 5) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (nz2 - 5));
 # 8 "<preprocessor>"
 hmpprt::s32 k_1;
 # 402 "rhs.c"
 # 402 "rhs.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 9 "<preprocessor>"
  hmpprt::s32 end_3;
  # 9 "<preprocessor>"
  hmpprt::s32 j_1;
  # 404 "rhs.c"
  # 404 "rhs.c"
  # 404 "rhs.c"
  for (j_1 = (hmpprt::gr_btidx()), end_3 = ny2_1 - 1 ; j_1 <= end_3 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
  {
   # 10 "<preprocessor>"
   hmpprt::s32 end_2;
   # 10 "<preprocessor>"
   hmpprt::s32 i_2;
   # 406 "rhs.c"
   # 406 "rhs.c"
   # 406 "rhs.c"
   for (i_2 = 0, end_2 = nx2 - 1 ; i_2 <= end_2 ; i_2 = i_2 + 1)
   {
    # 7 "<preprocessor>"
    hmpprt::s32 m_1;
    # 407 "rhs.c"
    # 407 "rhs.c"
    for (m_1 = 0 ; m_1 <= 4 ; m_1 = m_1 + 1)
    {
     # 411 "rhs.c"
     *(rhs + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 3 + 36uLL * m_1))) = *(rhs + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 3 + 36uLL * m_1))) - dssp * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) - (double) 4.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2 + 36uLL * m_1))) + (double) 6.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 3 + 36uLL * m_1))) - (double) 4.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 4 + 36uLL * m_1))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 5 + 36uLL * m_1))));
    }
    # 5 "<preprocessor>"
    # 5 "<preprocessor>"
    *m = 5;
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
void __hmpp_acc_region__compute_rhs_401__1cirzbhs_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, hmpprt::s32 nz2_2, double dssp_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_1, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32* __hmpp_addr__m_1)
{
 # 7 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  m_2;
 # 7 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&m_2), hmpprt::MS_CUDA_GLOB, 4);
 # 7 "<preprocessor>"
 *m_2 = *__hmpp_addr__m_1;
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
 # 36 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter(&dssp_2, 8, "dssp");
  __hmppcg_call.addLocalParameter(&m_2, 8, "m");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2_2), "nx2");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2_2), "ny2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2_2), "nz2");
  __hmppcg_call.addLocalParameter(&rhs_1, 8, "rhs");
  __hmppcg_call.addLocalParameter(&u_1, 8, "u");
  __hmppcg_call.launch(__hmpp_acc_region__compute_rhs_401__1cirzbhs_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 36 "<preprocessor>"
 *__hmpp_addr__i = i_1;
 # 37 "<preprocessor>"
 *__hmpp_addr__j_1 = j;
 # 38 "<preprocessor>"
 *__hmpp_addr__k_1 = k;
 # 39 "<preprocessor>"
 *__hmpp_addr__m_1 = *m_2;
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&m_2));
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_401__1cirzbhs(hmpprt::s32 nx2_1, hmpprt::s32 ny2, hmpprt::s32 nz2_1, double dssp_1, double* u_2, double* rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, hmpprt::s32* __hmpp_addr__m)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__compute_rhs_401__1cirzbhs_internal_1(nx2_1, ny2, nz2_1, dssp_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (u_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs_2), __hmpp_addr__i_1, __hmpp_addr__j, __hmpp_addr__k, __hmpp_addr__m));
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
      __hmpp_acc_region__compute_rhs_401__1cirzbhs_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__compute_rhs_401__1cirzbhs_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__compute_rhs_401__1cirzbhs", "prototype __hmpp_acc_region__compute_rhs_401__1cirzbhs(nx2: s32, ny2: s32, nz2: s32, dssp: double, u: ^cudaglob double, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32, __hmpp_addr__m: ^host s32)");

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
      delete __hmpp_acc_region__compute_rhs_401__1cirzbhs_parallel_region_1;

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
