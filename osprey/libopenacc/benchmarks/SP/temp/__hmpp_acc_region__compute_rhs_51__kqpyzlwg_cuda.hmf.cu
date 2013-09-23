
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

# 7 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_51__kqpyzlwg(hmpprt::s32* grid_points, double* u_1, double* us, double* vs_2, double* ws_1, double* qs, double* rho_i_1, double* square, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k_1)
;
#endif // __CUDACC__



# 7 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_51__kqpyzlwg_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  grid_points_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  us_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  vs, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  ws_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  qs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rho_i_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  square_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k)
;
#endif // __CUDACC__



# 7 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__compute_rhs_51__kqpyzlwg_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__compute_rhs_51__kqpyzlwg_parallel_region_1(hmpprt::s32* grid_points_1, double* qs_1, double* rho_i, double* square_1, double* u, double* us_1, double* vs_1, double* ws);
#endif // __CUDACC__




# 7 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__compute_rhs_51__kqpyzlwg_parallel_region_1(hmpprt::s32* grid_points_1, double* qs_1, double* rho_i, double* square_1, double* u, double* us_1, double* vs_1, double* ws)
{
 # 11 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 11 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 11 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 11 "<preprocessor>"
 iter_per_gang_1 = 1 + (*(grid_points_1 + 2) - 1) / 192;
 # 11 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 11 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (*(grid_points_1 + 2) - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (*(grid_points_1 + 2) - 1));
 # 11 "<preprocessor>"
 hmpprt::s32 k_1;
 # 52 "rhs.c"
 # 52 "rhs.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 12 "<preprocessor>"
  hmpprt::s32 end_2;
  # 12 "<preprocessor>"
  hmpprt::s32 j_1;
  # 54 "rhs.c"
  # 54 "rhs.c"
  # 54 "rhs.c"
  for (j_1 = (hmpprt::gr_btidx()), end_2 = *(grid_points_1 + 1) - 1 ; j_1 <= end_2 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
  {
   # 13 "<preprocessor>"
   hmpprt::s32 end_1;
   # 13 "<preprocessor>"
   hmpprt::s32 i_2;
   # 56 "rhs.c"
   # 56 "rhs.c"
   # 56 "rhs.c"
   for (i_2 = 0, end_1 = *grid_points_1 - 1 ; i_2 <= end_1 ; i_2 = i_2 + 1)
   {
    # 57 "rhs.c"
    double rho_inv_1;
    # 57 "rhs.c"
    rho_inv_1 = (double) 1.0 / *(u + i_2 + 37uLL * (j_1 + 37uLL * k_1));
    # 58 "rhs.c"
    *(rho_i + i_2 + 37uLL * (j_1 + 37uLL * k_1)) = rho_inv_1;
    # 59 "rhs.c"
    *(us_1 + i_2 + 37uLL * (j_1 + 37uLL * k_1)) = *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 36uLL))) * rho_inv_1;
    # 60 "rhs.c"
    *(vs_1 + i_2 + 37uLL * (j_1 + 37uLL * k_1)) = *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 72uLL))) * rho_inv_1;
    # 61 "rhs.c"
    *(ws + i_2 + 37uLL * (j_1 + 37uLL * k_1)) = *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 108uLL))) * rho_inv_1;
    # 65 "rhs.c"
    *(square_1 + i_2 + 37uLL * (j_1 + 37uLL * k_1)) = (double) 0.5 * (*(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 36uLL))) * *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 36uLL))) + *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 72uLL))) * *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 72uLL))) + *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 108uLL))) * *(u + i_2 + 37uLL * (j_1 + 37uLL * (k_1 + 108uLL)))) * rho_inv_1;
    # 66 "rhs.c"
    *(qs_1 + i_2 + 37uLL * (j_1 + 37uLL * k_1)) = *(square_1 + i_2 + 37uLL * (j_1 + 37uLL * k_1)) * rho_inv_1;
   }
   # 7 "<preprocessor>"
  }
  # 7 "<preprocessor>"
 }
 # 7 "<preprocessor>"
}
#endif // __CUDACC__



# 7 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_51__kqpyzlwg_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  grid_points_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  us_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  vs, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  ws_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  qs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rho_i_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  square_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k)
{
 # 11 "<preprocessor>"
 hmpprt::s32 k;
 # 12 "<preprocessor>"
 hmpprt::s32 j;
 # 12 "<preprocessor>"
 j = *__hmpp_addr__j_1;
 # 13 "<preprocessor>"
 hmpprt::s32 i_1;
 # 13 "<preprocessor>"
 i_1 = *__hmpp_addr__i_1;
 # 50 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter(&grid_points_2, 8, "grid_points_1");
  __hmppcg_call.addLocalParameter(&qs_2, 8, "qs_1");
  __hmppcg_call.addLocalParameter(&rho_i_2, 8, "rho_i");
  __hmppcg_call.addLocalParameter(&square_2, 8, "square_1");
  __hmppcg_call.addLocalParameter(&u_2, 8, "u");
  __hmppcg_call.addLocalParameter(&us_2, 8, "us_1");
  __hmppcg_call.addLocalParameter(&vs, 8, "vs_1");
  __hmppcg_call.addLocalParameter(&ws_2, 8, "ws");
  __hmppcg_call.launch(__hmpp_acc_region__compute_rhs_51__kqpyzlwg_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 50 "<preprocessor>"
 *__hmpp_addr__i_1 = i_1;
 # 51 "<preprocessor>"
 *__hmpp_addr__j_1 = j;
 # 52 "<preprocessor>"
 *__hmpp_addr__k = k;
}
#endif // __CUDACC__



# 7 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_51__kqpyzlwg(hmpprt::s32* grid_points, double* u_1, double* us, double* vs_2, double* ws_1, double* qs, double* rho_i_1, double* square, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k_1)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__compute_rhs_51__kqpyzlwg_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32> (grid_points), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (u_1), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (us), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (vs_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (ws_1), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (qs), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rho_i_1), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (square), __hmpp_addr__i, __hmpp_addr__j, __hmpp_addr__k_1));
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
      __hmpp_acc_region__compute_rhs_51__kqpyzlwg_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__compute_rhs_51__kqpyzlwg_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__compute_rhs_51__kqpyzlwg", "prototype __hmpp_acc_region__compute_rhs_51__kqpyzlwg(grid_points: ^cudaglob s32, u: ^cudaglob double, us: ^cudaglob double, vs: ^cudaglob double, ws: ^cudaglob double, qs: ^cudaglob double, rho_i: ^cudaglob double, square: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32)");

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
      delete __hmpp_acc_region__compute_rhs_51__kqpyzlwg_parallel_region_1;

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
