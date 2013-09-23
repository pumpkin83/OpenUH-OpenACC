
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
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_330__wsmtc0wg(hmpprt::s32 nx2, hmpprt::s32 ny2, hmpprt::s32 nz2_1, double c1_1, double c2, double con43_2, double* u_2, double* us_2, double* vs, double* ws_2, double* qs, double* rho_i_2, double* square_2, double* rhs_2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_330__wsmtc0wg_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, hmpprt::s32 nz2_2, double c1_2, double c2_2, double con43, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  us, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  vs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  ws_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  qs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rho_i, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  square_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__compute_rhs_330__wsmtc0wg_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__compute_rhs_330__wsmtc0wg_parallel_region_1(double c1, double c2_1, double con43_1, double dz1tz1_1, double dz2tz1_1, double dz3tz1, double dz4tz1, double dz5tz1_1, hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, hmpprt::s32 nz2, double* qs_1, double* rho_i_1, double* rhs_1, double* square, double tz2, double* u, double* us_1, double* vs_1, double* ws, double zzcon2_1, double zzcon3_1, double zzcon4_1, double zzcon5_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__compute_rhs_330__wsmtc0wg_parallel_region_1(double c1, double c2_1, double con43_1, double dz1tz1_1, double dz2tz1_1, double dz3tz1, double dz4tz1, double dz5tz1_1, hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, hmpprt::s32 nz2, double* qs_1, double* rho_i_1, double* rhs_1, double* square, double tz2, double* u, double* us_1, double* vs_1, double* ws, double zzcon2_1, double zzcon3_1, double zzcon4_1, double zzcon5_1)
{
 # 10 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 10 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 10 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 10 "<preprocessor>"
 iter_per_gang_1 = 1 + (nz2 - 1) / 192;
 # 10 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 10 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (nz2 - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (nz2 - 1));
 # 10 "<preprocessor>"
 hmpprt::s32 k_1;
 # 331 "rhs.c"
 # 331 "rhs.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 11 "<preprocessor>"
  hmpprt::s32 end_2;
  # 11 "<preprocessor>"
  hmpprt::s32 j_1;
  # 333 "rhs.c"
  # 333 "rhs.c"
  # 333 "rhs.c"
  for (j_1 = (hmpprt::gr_btidx()), end_2 = ny2_1 - 1 ; j_1 <= end_2 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
  {
   # 12 "<preprocessor>"
   hmpprt::s32 end_1;
   # 12 "<preprocessor>"
   hmpprt::s32 i_2;
   # 335 "rhs.c"
   # 335 "rhs.c"
   # 335 "rhs.c"
   for (i_2 = 0, end_1 = nx2_1 - 1 ; i_2 <= end_1 ; i_2 = i_2 + 1)
   {
    # 336 "rhs.c"
    double wp1_1;
    # 336 "rhs.c"
    double wm1_1;
    # 336 "rhs.c"
    double wijk_1;
    # 336 "rhs.c"
    wijk_1 = *(ws + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1)));
    # 337 "rhs.c"
    wp1_1 = *(ws + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2)));
    # 338 "rhs.c"
    wm1_1 = *(ws + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1));
    # 342 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + dz1tz1_1 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2))) - (double) 2.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1))) - tz2 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 110))) - *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 108))));
    # 347 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 37))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 37))) + dz2tz1_1 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 38))) - (double) 2.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 37))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 36)))) + zzcon2_1 * (*(us_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2))) - (double) 2.0 * *(us_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(us_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1))) - tz2 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 38))) * wp1_1 - *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 36))) * wm1_1);
    # 352 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 73))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 73))) + dz3tz1 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 74))) - (double) 2.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 73))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 72)))) + zzcon2_1 * (*(vs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2))) - (double) 2.0 * *(vs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(vs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1))) - tz2 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 74))) * wp1_1 - *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 72))) * wm1_1);
    # 359 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 109))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 109))) + dz4tz1 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 110))) - (double) 2.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 109))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 108)))) + zzcon2_1 * con43_1 * (wp1_1 - (double) 2.0 * wijk_1 + wm1_1) - tz2 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 110))) * wp1_1 - *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 108))) * wm1_1 + (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 146))) - *(square + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2))) - *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 144))) + *(square + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1))) * c2_1);
    # 369 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) + dz5tz1_1 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 146))) - (double) 2.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 144)))) + zzcon3_1 * (*(qs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2))) - (double) 2.0 * *(qs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(qs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1))) + zzcon4_1 * (wp1_1 * wp1_1 - (double) 2.0 * wijk_1 * wijk_1 + wm1_1 * wm1_1) + zzcon5_1 * (*(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 146))) * *(rho_i_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2))) - (double) 2.0 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) * *(rho_i_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 144))) * *(rho_i_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1))) - tz2 * ((c1 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 146))) - c2_1 * *(square + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 2)))) * wp1_1 - (c1 * *(u + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 144))) - c2_1 * *(square + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * k_1))) * wm1_1);
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
void __hmpp_acc_region__compute_rhs_330__wsmtc0wg_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, hmpprt::s32 nz2_2, double c1_2, double c2_2, double con43, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  us, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  vs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  ws_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  qs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rho_i, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  square_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k)
{
 # 10 "<preprocessor>"
 hmpprt::s32 k;
 # 11 "<preprocessor>"
 hmpprt::s32 j;
 # 11 "<preprocessor>"
 j = *__hmpp_addr__j_1;
 # 12 "<preprocessor>"
 hmpprt::s32 i_1;
 # 12 "<preprocessor>"
 i_1 = *__hmpp_addr__i_1;
 # 13 "<preprocessor>"
 double dz5tz1;
 # 14 "<preprocessor>"
 double dz4tz1_1;
 # 15 "<preprocessor>"
 double dz3tz1_1;
 # 16 "<preprocessor>"
 double dz2tz1;
 # 17 "<preprocessor>"
 double dz1tz1;
 # 18 "<preprocessor>"
 double zzcon5;
 # 19 "<preprocessor>"
 double zzcon4;
 # 20 "<preprocessor>"
 double zzcon3;
 # 21 "<preprocessor>"
 double zzcon2;
 # 22 "<preprocessor>"
 double tz2_1;
 # 76 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter(&c1_2, 8, "c1");
  __hmppcg_call.addLocalParameter(&c2_2, 8, "c2_1");
  __hmppcg_call.addLocalParameter(&con43, 8, "con43_1");
  __hmppcg_call.addLocalParameter(&dz1tz1, 8, "dz1tz1_1");
  __hmppcg_call.addLocalParameter(&dz2tz1, 8, "dz2tz1_1");
  __hmppcg_call.addLocalParameter(&dz3tz1_1, 8, "dz3tz1");
  __hmppcg_call.addLocalParameter(&dz4tz1_1, 8, "dz4tz1");
  __hmppcg_call.addLocalParameter(&dz5tz1, 8, "dz5tz1_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2_2), "nx2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2_2), "ny2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2_2), "nz2");
  __hmppcg_call.addLocalParameter(&qs_2, 8, "qs_1");
  __hmppcg_call.addLocalParameter(&rho_i, 8, "rho_i_1");
  __hmppcg_call.addLocalParameter(&rhs, 8, "rhs_1");
  __hmppcg_call.addLocalParameter(&square_1, 8, "square");
  __hmppcg_call.addLocalParameter(&tz2_1, 8, "tz2");
  __hmppcg_call.addLocalParameter(&u_1, 8, "u");
  __hmppcg_call.addLocalParameter(&us, 8, "us_1");
  __hmppcg_call.addLocalParameter(&vs_2, 8, "vs_1");
  __hmppcg_call.addLocalParameter(&ws_1, 8, "ws");
  __hmppcg_call.addLocalParameter(&zzcon2, 8, "zzcon2_1");
  __hmppcg_call.addLocalParameter(&zzcon3, 8, "zzcon3_1");
  __hmppcg_call.addLocalParameter(&zzcon4, 8, "zzcon4_1");
  __hmppcg_call.addLocalParameter(&zzcon5, 8, "zzcon5_1");
  __hmppcg_call.launch(__hmpp_acc_region__compute_rhs_330__wsmtc0wg_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 76 "<preprocessor>"
 *__hmpp_addr__i_1 = i_1;
 # 77 "<preprocessor>"
 *__hmpp_addr__j_1 = j;
 # 78 "<preprocessor>"
 *__hmpp_addr__k = k;
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_330__wsmtc0wg(hmpprt::s32 nx2, hmpprt::s32 ny2, hmpprt::s32 nz2_1, double c1_1, double c2, double con43_2, double* u_2, double* us_2, double* vs, double* ws_2, double* qs, double* rho_i_2, double* square_2, double* rhs_2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k_1)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__compute_rhs_330__wsmtc0wg_internal_1(nx2, ny2, nz2_1, c1_1, c2, con43_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (u_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (us_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (vs), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (ws_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (qs), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rho_i_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (square_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs_2), __hmpp_addr__i, __hmpp_addr__j, __hmpp_addr__k_1));
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
      __hmpp_acc_region__compute_rhs_330__wsmtc0wg_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__compute_rhs_330__wsmtc0wg_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__compute_rhs_330__wsmtc0wg", "prototype __hmpp_acc_region__compute_rhs_330__wsmtc0wg(nx2: s32, ny2: s32, nz2: s32, c1: double, c2: double, con43: double, u: ^cudaglob double, us: ^cudaglob double, vs: ^cudaglob double, ws: ^cudaglob double, qs: ^cudaglob double, rho_i: ^cudaglob double, square: ^cudaglob double, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32)");

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
      delete __hmpp_acc_region__compute_rhs_330__wsmtc0wg_parallel_region_1;

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
