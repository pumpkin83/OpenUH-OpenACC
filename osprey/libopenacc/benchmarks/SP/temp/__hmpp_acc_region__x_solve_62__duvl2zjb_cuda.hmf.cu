
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
extern "C" CDLT_API  void __hmpp_acc_region__x_solve_62__duvl2zjb(hmpprt::u64* __hmpp_vla_sizes__rhsX_2, hmpprt::s32 nz2_2, double* rhs, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, double* rhsX)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__x_solve_62__duvl2zjb_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64>  __hmpp_vla_sizes__rhsX, hmpprt::s32 nz2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhsX_2)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__x_solve_62__duvl2zjb_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__x_solve_62__duvl2zjb_parallel_region_1(hmpprt::s32* i_3, hmpprt::s32* j, hmpprt::s32 nz2_1, double* rhs_1, hmpprt::u64* __hmpp_vla_sizes__rhsX_1, double* rhsX_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__x_solve_62__duvl2zjb_parallel_region_1(hmpprt::s32* i_3, hmpprt::s32* j, hmpprt::s32 nz2_1, double* rhs_1, hmpprt::u64* __hmpp_vla_sizes__rhsX_1, double* rhsX_1)
{
 # 7 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 7 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 7 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 7 "<preprocessor>"
 iter_per_gang_1 = ((1 + nz2_1 / 192) > 8LL ? (1 + nz2_1 / 192) : 8LL);
 # 7 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 7 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < nz2_1 ? (first_gang_iter_1 + iter_per_gang_1 - 1) : nz2_1);
 # 7 "<preprocessor>"
 hmpprt::s32 k_1;
 # 63 "x_solve.c"
 # 63 "x_solve.c"
 for (k_1 = first_gang_iter_1 + (hmpprt::gr_btidy()) ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + (hmpprt::gr_btnumy()))
 {
  # 8 "<preprocessor>"
  hmpprt::s32 j_1;
  # 64 "x_solve.c"
  # 64 "x_solve.c"
  for (j_1 = 0 ; j_1 <= 36 ; j_1 = j_1 + 1)
  {
   # 9 "<preprocessor>"
   hmpprt::s32 i_2;
   # 65 "x_solve.c"
   # 65 "x_solve.c"
   for (i_2 = (hmpprt::gr_btidx()) ; i_2 <= 36 ; i_2 = i_2 + (hmpprt::gr_btnumx()))
   {
    # 66 "x_solve.c"
    *(rhsX_1 + j_1 + *(__hmpp_vla_sizes__rhsX_1 + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX_1 + 2) * k_1)) = *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * k_1));
    # 67 "x_solve.c"
    *(rhsX_1 + j_1 + *(__hmpp_vla_sizes__rhsX_1 + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX_1 + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX_1 + 1)))) = *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 36uLL)));
    # 68 "x_solve.c"
    *(rhsX_1 + j_1 + *(__hmpp_vla_sizes__rhsX_1 + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX_1 + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX_1 + 1) * 2))) = *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 72uLL)));
    # 69 "x_solve.c"
    *(rhsX_1 + j_1 + *(__hmpp_vla_sizes__rhsX_1 + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX_1 + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX_1 + 1) * 3))) = *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 108uLL)));
    # 70 "x_solve.c"
    *(rhsX_1 + j_1 + *(__hmpp_vla_sizes__rhsX_1 + 3) * (i_2 + *(__hmpp_vla_sizes__rhsX_1 + 2) * (k_1 + *(__hmpp_vla_sizes__rhsX_1 + 1) * 4))) = *(rhs_1 + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 144uLL)));
   }
   # 5 "<preprocessor>"
   # 5 "<preprocessor>"
   *i_3 = 37;
  }
  # 5 "<preprocessor>"
  # 5 "<preprocessor>"
  *j = 37;
 }
 # 5 "<preprocessor>"
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__x_solve_62__duvl2zjb_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64>  __hmpp_vla_sizes__rhsX, hmpprt::s32 nz2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs_2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhsX_2)
{
 # 7 "<preprocessor>"
 hmpprt::s32 k;
 # 8 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  j_2;
 # 8 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&j_2), hmpprt::MS_CUDA_GLOB, 4);
 # 8 "<preprocessor>"
 *j_2 = *__hmpp_addr__j_1;
 # 9 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  i_1;
 # 9 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&i_1), hmpprt::MS_CUDA_GLOB, 4);
 # 9 "<preprocessor>"
 *i_1 = *__hmpp_addr__i;
 # 26 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter(&i_1, 8, "i_3");
  __hmppcg_call.addLocalParameter(&j_2, 8, "j");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2), "nz2_1");
  __hmppcg_call.addLocalParameter(&rhs_2, 8, "rhs_1");
  __hmppcg_call.addLocalParameter(&__hmpp_vla_sizes__rhsX, 8, "__hmpp_vla_sizes__rhsX_1");
  __hmppcg_call.addLocalParameter(&rhsX_2, 8, "rhsX_1");
  __hmppcg_call.launch(__hmpp_acc_region__x_solve_62__duvl2zjb_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 26 "<preprocessor>"
 *__hmpp_addr__i = *i_1;
 # 27 "<preprocessor>"
 *__hmpp_addr__j_1 = *j_2;
 # 28 "<preprocessor>"
 *__hmpp_addr__k_1 = k;
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&j_2));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&i_1));
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__x_solve_62__duvl2zjb(hmpprt::u64* __hmpp_vla_sizes__rhsX_2, hmpprt::s32 nz2_2, double* rhs, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, double* rhsX)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__x_solve_62__duvl2zjb_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64> (__hmpp_vla_sizes__rhsX_2), nz2_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs), __hmpp_addr__i_1, __hmpp_addr__j, __hmpp_addr__k, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhsX)));
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
      __hmpp_acc_region__x_solve_62__duvl2zjb_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__x_solve_62__duvl2zjb_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__x_solve_62__duvl2zjb", "prototype __hmpp_acc_region__x_solve_62__duvl2zjb(__hmpp_vla_sizes__rhsX: ^cudaglob u64, nz2: s32, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32, rhsX: ^cudaglob double)");

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
      delete __hmpp_acc_region__x_solve_62__duvl2zjb_parallel_region_1;

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
