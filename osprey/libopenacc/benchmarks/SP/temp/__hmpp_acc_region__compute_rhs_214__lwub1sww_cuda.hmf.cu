
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
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_214__lwub1sww(hmpprt::s32 nx2, hmpprt::s32 ny2, hmpprt::s32 nz2, double c1_2, double c2_2, double con43_2, double* u_2, double* us_2, double* vs_2, double* ws_2, double* qs, double* rho_i_2, double* square, double* rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__compute_rhs_214__lwub1sww_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, hmpprt::s32 nz2_2, double c1, double c2, double con43, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  us_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  vs, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  ws, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  qs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rho_i, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  square_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__compute_rhs_214__lwub1sww_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__compute_rhs_214__lwub1sww_parallel_region_1(double c1_1, double c2_1, double con43_1, double dy1ty1_1, double dy2ty1_1, double dy3ty1_1, double dy4ty1_1, double dy5ty1_1, hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, hmpprt::s32 nz2_1, double* qs_1, double* rho_i_1, double* rhs_1, double* square_1, double ty2_1, double* u_1, double* us, double* vs_1, double* ws_1, double yycon2_1, double yycon3_1, double yycon4_1, double yycon5_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__compute_rhs_214__lwub1sww_parallel_region_1(double c1_1, double c2_1, double con43_1, double dy1ty1_1, double dy2ty1_1, double dy3ty1_1, double dy4ty1_1, double dy5ty1_1, hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, hmpprt::s32 nz2_1, double* qs_1, double* rho_i_1, double* rhs_1, double* square_1, double ty2_1, double* u_1, double* us, double* vs_1, double* ws_1, double yycon2_1, double yycon3_1, double yycon4_1, double yycon5_1)
{
 # 10 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 10 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 10 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 10 "<preprocessor>"
 iter_per_gang_1 = 1 + (nz2_1 - 1) / 192;
 # 10 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 10 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (nz2_1 - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (nz2_1 - 1));
 # 10 "<preprocessor>"
 hmpprt::s32 k_1;
 # 215 "rhs.c"
 # 215 "rhs.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 11 "<preprocessor>"
  hmpprt::s32 end_2;
  # 11 "<preprocessor>"
  hmpprt::s32 j_1;
  # 217 "rhs.c"
  # 217 "rhs.c"
  # 217 "rhs.c"
  for (j_1 = (hmpprt::gr_btidx()), end_2 = ny2_1 - 1 ; j_1 <= end_2 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
  {
   # 12 "<preprocessor>"
   hmpprt::s32 end_1;
   # 12 "<preprocessor>"
   hmpprt::s32 i_2;
   # 219 "rhs.c"
   # 219 "rhs.c"
   # 219 "rhs.c"
   for (i_2 = 0, end_1 = nx2_1 - 1 ; i_2 <= end_1 ; i_2 = i_2 + 1)
   {
    # 220 "rhs.c"
    double vp1_1;
    # 220 "rhs.c"
    double vm1_1;
    # 220 "rhs.c"
    double vijk_1;
    # 220 "rhs.c"
    vijk_1 = *(vs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1)));
    # 221 "rhs.c"
    vp1_1 = *(vs_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1)));
    # 222 "rhs.c"
    vm1_1 = *(vs_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)));
    # 226 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + dy1ty1_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1))) - (double) 2.0 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)))) - ty2_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 73))) - *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 73))));
    # 231 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 37))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 37))) + dy2ty1_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 37))) - (double) 2.0 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 37))) + *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 37)))) + yycon2_1 * (*(us + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1))) - (double) 2.0 * *(us + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(us + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)))) - ty2_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 37))) * vp1_1 - *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 37))) * vm1_1);
    # 238 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 73))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 73))) + dy3ty1_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 73))) - (double) 2.0 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 73))) + *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 73)))) + yycon2_1 * con43_1 * (vp1_1 - (double) 2.0 * vijk_1 + vm1_1) - ty2_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 73))) * vp1_1 - *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 73))) * vm1_1 + (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 145))) - *(square_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1))) - *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 145))) + *(square_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)))) * c2_1);
    # 243 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 109))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 109))) + dy4ty1_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 109))) - (double) 2.0 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 109))) + *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 109)))) + yycon2_1 * (*(ws_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1))) - (double) 2.0 * *(ws_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(ws_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)))) - ty2_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 109))) * vp1_1 - *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 109))) * vm1_1);
    # 253 "rhs.c"
    *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) = *(rhs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) + dy5ty1_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 145))) - (double) 2.0 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) + *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 145)))) + yycon3_1 * (*(qs_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1))) - (double) 2.0 * *(qs_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(qs_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)))) + yycon4_1 * (vp1_1 * vp1_1 - (double) 2.0 * vijk_1 * vijk_1 + vm1_1 * vm1_1) + yycon5_1 * (*(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 145))) * *(rho_i_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1))) - (double) 2.0 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 145))) * *(rho_i_1 + (i_2 + 1) + 37uLL * (j_1 + 1 + 37uLL * (k_1 + 1))) + *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 145))) * *(rho_i_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)))) - ty2_1 * ((c1_1 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 145))) - c2_1 * *(square_1 + (i_2 + 1) + 37uLL * (j_1 + 2 + 37uLL * (k_1 + 1)))) * vp1_1 - (c1_1 * *(u_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 145))) - c2_1 * *(square_1 + (i_2 + 1) + 37uLL * (j_1 + 37uLL * (k_1 + 1)))) * vm1_1);
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
void __hmpp_acc_region__compute_rhs_214__lwub1sww_internal_1(hmpprt::s32 nx2_2, hmpprt::s32 ny2_2, hmpprt::s32 nz2_2, double c1, double c2, double con43, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  u, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  us_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  vs, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  ws, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  qs_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rho_i, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  square_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhs, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k)
{
 # 10 "<preprocessor>"
 hmpprt::s32 k;
 # 11 "<preprocessor>"
 hmpprt::s32 j;
 # 11 "<preprocessor>"
 j = *__hmpp_addr__j;
 # 12 "<preprocessor>"
 hmpprt::s32 i_1;
 # 12 "<preprocessor>"
 i_1 = *__hmpp_addr__i;
 # 13 "<preprocessor>"
 double dy5ty1;
 # 14 "<preprocessor>"
 double dy4ty1;
 # 15 "<preprocessor>"
 double dy3ty1;
 # 16 "<preprocessor>"
 double dy2ty1;
 # 17 "<preprocessor>"
 double dy1ty1;
 # 18 "<preprocessor>"
 double yycon5;
 # 19 "<preprocessor>"
 double yycon4;
 # 20 "<preprocessor>"
 double yycon3;
 # 21 "<preprocessor>"
 double yycon2;
 # 22 "<preprocessor>"
 double ty2;
 # 76 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(1);
  __hmppcg_call.addLocalParameter(&c1, 8, "c1_1");
  __hmppcg_call.addLocalParameter(&c2, 8, "c2_1");
  __hmppcg_call.addLocalParameter(&con43, 8, "con43_1");
  __hmppcg_call.addLocalParameter(&dy1ty1, 8, "dy1ty1_1");
  __hmppcg_call.addLocalParameter(&dy2ty1, 8, "dy2ty1_1");
  __hmppcg_call.addLocalParameter(&dy3ty1, 8, "dy3ty1_1");
  __hmppcg_call.addLocalParameter(&dy4ty1, 8, "dy4ty1_1");
  __hmppcg_call.addLocalParameter(&dy5ty1, 8, "dy5ty1_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2_2), "nx2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2_2), "ny2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2_2), "nz2_1");
  __hmppcg_call.addLocalParameter(&qs_2, 8, "qs_1");
  __hmppcg_call.addLocalParameter(&rho_i, 8, "rho_i_1");
  __hmppcg_call.addLocalParameter(&rhs, 8, "rhs_1");
  __hmppcg_call.addLocalParameter(&square_2, 8, "square_1");
  __hmppcg_call.addLocalParameter(&ty2, 8, "ty2_1");
  __hmppcg_call.addLocalParameter(&u, 8, "u_1");
  __hmppcg_call.addLocalParameter(&us_1, 8, "us");
  __hmppcg_call.addLocalParameter(&vs, 8, "vs_1");
  __hmppcg_call.addLocalParameter(&ws, 8, "ws_1");
  __hmppcg_call.addLocalParameter(&yycon2, 8, "yycon2_1");
  __hmppcg_call.addLocalParameter(&yycon3, 8, "yycon3_1");
  __hmppcg_call.addLocalParameter(&yycon4, 8, "yycon4_1");
  __hmppcg_call.addLocalParameter(&yycon5, 8, "yycon5_1");
  __hmppcg_call.launch(__hmpp_acc_region__compute_rhs_214__lwub1sww_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 76 "<preprocessor>"
 *__hmpp_addr__i = i_1;
 # 77 "<preprocessor>"
 *__hmpp_addr__j = j;
 # 78 "<preprocessor>"
 *__hmpp_addr__k = k;
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__compute_rhs_214__lwub1sww(hmpprt::s32 nx2, hmpprt::s32 ny2, hmpprt::s32 nz2, double c1_2, double c2_2, double con43_2, double* u_2, double* us_2, double* vs_2, double* ws_2, double* qs, double* rho_i_2, double* square, double* rhs_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__compute_rhs_214__lwub1sww_internal_1(nx2, ny2, nz2, c1_2, c2_2, con43_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (u_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (us_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (vs_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (ws_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (qs), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rho_i_2), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (square), hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhs_2), __hmpp_addr__i_1, __hmpp_addr__j_1, __hmpp_addr__k_1));
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
      __hmpp_acc_region__compute_rhs_214__lwub1sww_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__compute_rhs_214__lwub1sww_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__compute_rhs_214__lwub1sww", "prototype __hmpp_acc_region__compute_rhs_214__lwub1sww(nx2: s32, ny2: s32, nz2: s32, c1: double, c2: double, con43: double, u: ^cudaglob double, us: ^cudaglob double, vs: ^cudaglob double, ws: ^cudaglob double, qs: ^cudaglob double, rho_i: ^cudaglob double, square: ^cudaglob double, rhs: ^cudaglob double, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32)");

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
      delete __hmpp_acc_region__compute_rhs_214__lwub1sww_parallel_region_1;

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
