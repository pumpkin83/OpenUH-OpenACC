
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
extern "C" CDLT_API  void __hmpp_acc_region__x_solve_347__5ivlez75(hmpprt::u64* __hmpp_vla_sizes__rhsX_1, hmpprt::s32 nz2_1, double* rhs, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k_1, double* rhsX_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__x_solve_347__5ivlez75_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64>  __hmpp_vla_sizes__rhsX_2, hmpprt::s32 nz2_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhsX_2)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__x_solve_347__5ivlez75_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__x_solve_347__5ivlez75_parallel_region_1(hmpprt::s32 nz2, double* rhs_1, hmpprt::u64* __hmpp_vla_sizes__rhsX, double* rhsX);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__x_solve_347__5ivlez75_parallel_region_1(hmpprt::s32 nz2, double* rhs_1, hmpprt::u64* __hmpp_vla_sizes__rhsX, double* rhsX)
{
 # 7 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 7 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 7 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 7 "<preprocessor>"
 iter_per_gang_1 = 1 + nz2 / 192;
 # 7 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 7 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < nz2 ? (first_gang_iter_1 + iter_per_gang_1 - 1) : nz2);
 # 7 "<preprocessor>"
 hmpprt::s32 k_1;
 # 348 "x_solve.c"
 # 348 "x_solve.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 8 "<preprocessor>"
  hmpprt::s32 j_1;
  # 350 "x_solve.c"
  # 350 "x_solve.c"
  for (j_1 = (hmpprt::gr_btidx()) ; j_1 <= 36 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
  {
   # 9 "<preprocessor>"
   hmpprt::s32 i_2;
   # 352 "x_solve.c"
   # 352 "x_solve.c"
   for (i_2 = 0 ; i_2 <= 36 ; i_2 = i_2 + 1)
   {
    # 353 "x_solve.c"
    *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * k_1)) = *(rhsX + j_1 + *(__hmpp_vla_sizes__rhsX + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX + 2) * k_1));
    # 354 "x_solve.c"
    *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 36uLL))) = *(rhsX + j_1 + *(__hmpp_vla_sizes__rhsX + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX + 1))));
    # 355 "x_solve.c"
    *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 72uLL))) = *(rhsX + j_1 + *(__hmpp_vla_sizes__rhsX + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX + 1) * 2)));
    # 356 "x_solve.c"
    *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 108uLL))) = *(rhsX + j_1 + *(__hmpp_vla_sizes__rhsX + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
    # 357 "x_solve.c"
    *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 144uLL))) = *(rhsX + j_1 + *(__hmpp_vla_sizes__rhsX + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
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
void __hmpp_acc_region__x_solve_347__5ivlez75_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64>  __hmpp_vla_sizes__rhsX_2, hmpprt::s32 nz2_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhsX_2)
{
 # 7 "<preprocessor>"
 hmpprt::s32 k;
 # 8 "<preprocessor>"
 hmpprt::s32 j;
 # 8 "<preprocessor>"
 j = *__hmpp_addr__j_1;
 # 9 "<preprocessor>"
 hmpprt::s32 i_1;
 # 9 "<preprocessor>"
 i_1 = *__hmpp_addr__i_1;
 # 34 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2_2), "nz2");
  __hmppcg_call.addLocalParameter(&rhs_2, 8, "rhs_1");
  __hmppcg_call.addLocalParameter(&__hmpp_vla_sizes__rhsX_2, 8, "__hmpp_vla_sizes__rhsX");
  __hmppcg_call.addLocalParameter(&rhsX_2, 8, "rhsX");
  __hmppcg_call.launch(__hmpp_acc_region__x_solve_347__5ivlez75_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 34 "<preprocessor>"
 *__hmpp_addr__i_1 = i_1;
 # 35 "<preprocessor>"
 *__hmpp_addr__j_1 = j;
 # 36 "<preprocessor>"
 *__hmpp_addr__k = k;
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__x_solve_347__5ivlez75(hmpprt::u64* __hmpp_vla_sizes__rhsX_1, hmpprt::s32 nz2_1, double* rhs, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k_1, double* rhsX_1)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__x_solve_347__5ivlez75_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64> (__hmpp_vla_sizes__rhsX_1), nz2_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs), __hmpp_addr__i, __hmpp_addr__j, __hmpp_addr__k_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhsX_1)));
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
      __hmpp_acc_region__x_solve_347__5ivlez75_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__x_solve_347__5ivlez75_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__x_solve_347__5ivlez75", "prototype __hmpp_acc_region__x_solve_347__5ivlez75(__hmpp_vla_sizes__rhsX: ^cudaglob u64, nz2: s32, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32, rhsX: ^cudaglob double)");

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
      delete __hmpp_acc_region__x_solve_347__5ivlez75_parallel_region_1;

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
