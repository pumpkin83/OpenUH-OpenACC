
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
extern "C" CDLT_API  void __hmpp_acc_region__y_solve_64__mhml6p0i(hmpprt::s32 ni_2, hmpprt::s32 nj_1, hmpprt::s32 gp0_2, hmpprt::s32 gp1, hmpprt::s32 gp2_2)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__y_solve_64__mhml6p0i_internal_1(hmpprt::s32 ni, hmpprt::s32 nj, hmpprt::s32 gp0, hmpprt::s32 gp1_2, hmpprt::s32 gp2)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__y_solve_64__mhml6p0i_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__y_solve_64__mhml6p0i_parallel_region_1(hmpprt::s32 gp0_1, hmpprt::s32 gp1_1, hmpprt::s32 gp2_1, hmpprt::s32 ni_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__y_solve_64__mhml6p0i_parallel_region_1(hmpprt::s32 gp0_1, hmpprt::s32 gp1_1, hmpprt::s32 gp2_1, hmpprt::s32 ni_1)
{
 # 17 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 17 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 17 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 17 "<preprocessor>"
 iter_per_gang_1 = 1 + (gp2_1 - 3) / 192;
 # 17 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 17 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (gp2_1 - 3) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (gp2_1 - 3));
 # 17 "<preprocessor>"
 hmpprt::s32 k_1;
 # 66 "y_solve.c"
 # 66 "y_solve.c"
 for (k_1 = first_gang_iter_1 ; k_1 <= last_gang_iter_1 ; k_1 = k_1 + 1)
 {
  # 19 "<preprocessor>"
  hmpprt::s32 end_2;
  # 19 "<preprocessor>"
  hmpprt::s32 i_2;
  # 68 "y_solve.c"
  # 68 "y_solve.c"
  # 68 "y_solve.c"
  for (i_2 = (hmpprt::gr_btidy()), end_2 = ni_1 - 1 ; i_2 <= end_2 ; i_2 = i_2 + (hmpprt::gr_btnumy()))
  {
   # 14 "<preprocessor>"
   hmpprt::s32 m_1;
   # 69 "y_solve.c"
   # 69 "y_solve.c"
   for (m_1 = (hmpprt::gr_btidx()) ; m_1 <= 4 ; m_1 = m_1 + (hmpprt::gr_btnumx()))
   {
    }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_5;
  # 19 "<preprocessor>"
  hmpprt::s32 i_3;
  # 92 "y_solve.c"
  # 92 "y_solve.c"
  # 92 "y_solve.c"
  for (i_3 = (hmpprt::gr_btidy()), end_5 = gp0_1 - 3 ; i_3 <= end_5 ; i_3 = i_3 + (hmpprt::gr_btnumy()))
  {
   # 18 "<preprocessor>"
   hmpprt::s32 end_3;
   # 18 "<preprocessor>"
   hmpprt::s32 j_1;
   # 93 "y_solve.c"
   # 93 "y_solve.c"
   # 93 "y_solve.c"
   for (j_1 = (hmpprt::gr_btidx()), end_3 = gp1_1 - 1 ; j_1 <= end_3 ; j_1 = j_1 + (hmpprt::gr_btnumx()))
   {
    }
   # 18 "<preprocessor>"
   # 18 "<preprocessor>"
   hmpprt::s32 end_4;
   # 18 "<preprocessor>"
   hmpprt::s32 j_2;
   # 99 "y_solve.c"
   # 99 "y_solve.c"
   # 99 "y_solve.c"
   for (j_2 = (hmpprt::gr_btidx()), end_4 = gp1_1 - 3 ; j_2 <= end_4 ; j_2 = j_2 + (hmpprt::gr_btnumx()))
   {
    }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_6;
  # 19 "<preprocessor>"
  hmpprt::s32 i_4;
  # 114 "y_solve.c"
  # 114 "y_solve.c"
  # 114 "y_solve.c"
  for (i_4 = (hmpprt::gr_btidy()), end_6 = gp0_1 - 3 ; i_4 <= end_6 ; i_4 = i_4 + (hmpprt::gr_btnumy()))
  {
   }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_8;
  # 18 "<preprocessor>"
  hmpprt::s32 j_3;
  # 126 "y_solve.c"
  # 126 "y_solve.c"
  # 126 "y_solve.c"
  for (j_3 = (hmpprt::gr_btidy()), end_8 = gp1_1 - 7 ; j_3 <= end_8 ; j_3 = j_3 + (hmpprt::gr_btnumy()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_7;
   # 19 "<preprocessor>"
   hmpprt::s32 i_5;
   # 127 "y_solve.c"
   # 127 "y_solve.c"
   # 127 "y_solve.c"
   for (i_5 = (hmpprt::gr_btidx()), end_7 = gp0_1 - 3 ; i_5 <= end_7 ; i_5 = i_5 + (hmpprt::gr_btnumx()))
   {
    }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_9;
  # 19 "<preprocessor>"
  hmpprt::s32 i_6;
  # 136 "y_solve.c"
  # 136 "y_solve.c"
  # 136 "y_solve.c"
  for (i_6 = (hmpprt::gr_btidy()), end_9 = gp0_1 - 3 ; i_6 <= end_9 ; i_6 = i_6 + (hmpprt::gr_btnumy()))
  {
   }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_11;
  # 18 "<preprocessor>"
  hmpprt::s32 j_4;
  # 151 "y_solve.c"
  # 151 "y_solve.c"
  # 151 "y_solve.c"
  for (j_4 = (hmpprt::gr_btidy()), end_11 = gp1_1 - 3 ; j_4 <= end_11 ; j_4 = j_4 + (hmpprt::gr_btnumy()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_10;
   # 19 "<preprocessor>"
   hmpprt::s32 i_7;
   # 152 "y_solve.c"
   # 152 "y_solve.c"
   # 152 "y_solve.c"
   for (i_7 = (hmpprt::gr_btidx()), end_10 = gp0_1 - 3 ; i_7 <= end_10 ; i_7 = i_7 + (hmpprt::gr_btnumx()))
   {
    }
   # 18 "<preprocessor>"
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_16;
  # 18 "<preprocessor>"
  hmpprt::s32 j_5;
  # 170 "y_solve.c"
  # 170 "y_solve.c"
  # 170 "y_solve.c"
  for (j_5 = (hmpprt::gr_btidy()), end_16 = gp1_1 - 3 ; j_5 <= end_16 ; j_5 = j_5 + (hmpprt::gr_btnumy()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_15;
   # 19 "<preprocessor>"
   hmpprt::s32 i_8;
   # 173 "y_solve.c"
   # 173 "y_solve.c"
   # 173 "y_solve.c"
   for (i_8 = (hmpprt::gr_btidx()), end_15 = gp0_1 - 3 ; i_8 <= end_15 ; i_8 = i_8 + (hmpprt::gr_btnumx()))
   {
    # 14 "<preprocessor>"
    hmpprt::s32 m_2;
    # 177 "y_solve.c"
    # 177 "y_solve.c"
    for (m_2 = 0 ; m_2 <= 2 ; m_2 = m_2 + 1)
    {
     }
    # 14 "<preprocessor>"
    # 14 "<preprocessor>"
    hmpprt::s32 m_3;
    # 182 "y_solve.c"
    # 182 "y_solve.c"
    for (m_3 = 0 ; m_3 <= 2 ; m_3 = m_3 + 1)
    {
     }
    # 14 "<preprocessor>"
    # 14 "<preprocessor>"
    hmpprt::s32 m_4;
    # 187 "y_solve.c"
    # 187 "y_solve.c"
    for (m_4 = 0 ; m_4 <= 2 ; m_4 = m_4 + 1)
    {
     }
    # 19 "<preprocessor>"
   }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_20;
  # 19 "<preprocessor>"
  hmpprt::s32 i_9;
  # 200 "y_solve.c"
  # 200 "y_solve.c"
  # 200 "y_solve.c"
  for (i_9 = (hmpprt::gr_btidy()), end_20 = gp0_1 - 3 ; i_9 <= end_20 ; i_9 = i_9 + (hmpprt::gr_btnumy()))
  {
   # 14 "<preprocessor>"
   hmpprt::s32 m_5;
   # 204 "y_solve.c"
   # 204 "y_solve.c"
   for (m_5 = (hmpprt::gr_btidx()) ; m_5 <= 2 ; m_5 = m_5 + (hmpprt::gr_btnumx()))
   {
    }
   # 14 "<preprocessor>"
   # 14 "<preprocessor>"
   hmpprt::s32 m_6;
   # 209 "y_solve.c"
   # 209 "y_solve.c"
   for (m_6 = (hmpprt::gr_btidx()) ; m_6 <= 2 ; m_6 = m_6 + (hmpprt::gr_btnumx()))
   {
    }
   # 14 "<preprocessor>"
   # 14 "<preprocessor>"
   hmpprt::s32 m_7;
   # 216 "y_solve.c"
   # 216 "y_solve.c"
   for (m_7 = (hmpprt::gr_btidx()) ; m_7 <= 2 ; m_7 = m_7 + (hmpprt::gr_btnumx()))
   {
    }
   # 18 "<preprocessor>"
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_22;
  # 18 "<preprocessor>"
  hmpprt::s32 j_6;
  # 224 "y_solve.c"
  # 224 "y_solve.c"
  # 224 "y_solve.c"
  for (j_6 = (hmpprt::gr_btidy()), end_22 = gp1_1 - 3 ; j_6 <= end_22 ; j_6 = j_6 + (hmpprt::gr_btnumy()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_21;
   # 19 "<preprocessor>"
   hmpprt::s32 i_10;
   # 227 "y_solve.c"
   # 227 "y_solve.c"
   # 227 "y_solve.c"
   for (i_10 = (hmpprt::gr_btidx()), end_21 = gp0_1 - 3 ; i_10 <= end_21 ; i_10 = i_10 + (hmpprt::gr_btnumx()))
   {
    }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_23;
  # 19 "<preprocessor>"
  hmpprt::s32 i_11;
  # 259 "y_solve.c"
  # 259 "y_solve.c"
  # 259 "y_solve.c"
  for (i_11 = (hmpprt::gr_btidy()), end_23 = gp0_1 - 3 ; i_11 <= end_23 ; i_11 = i_11 + (hmpprt::gr_btnumy()))
  {
   }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_25;
  # 19 "<preprocessor>"
  hmpprt::s32 i_12;
  # 291 "y_solve.c"
  # 291 "y_solve.c"
  # 291 "y_solve.c"
  for (i_12 = (hmpprt::gr_btidy()), end_25 = gp0_1 - 3 ; i_12 <= end_25 ; i_12 = i_12 + (hmpprt::gr_btnumy()))
  {
   # 14 "<preprocessor>"
   hmpprt::s32 m_8;
   # 292 "y_solve.c"
   # 292 "y_solve.c"
   for (m_8 = (hmpprt::gr_btidx()) ; m_8 <= 2 ; m_8 = m_8 + (hmpprt::gr_btnumx()))
   {
    }
   # 18 "<preprocessor>"
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_28;
  # 18 "<preprocessor>"
  hmpprt::s32 j_7;
  # 303 "y_solve.c"
  # 303 "y_solve.c"
  # 303 "y_solve.c"
  for (j_7 = (hmpprt::gr_btidy()), end_28 = gp1_1 - 3 ; j_7 <= end_28 ; j_7 = j_7 + (hmpprt::gr_btnumy()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_27;
   # 19 "<preprocessor>"
   hmpprt::s32 i_13;
   # 306 "y_solve.c"
   # 306 "y_solve.c"
   # 306 "y_solve.c"
   for (i_13 = (hmpprt::gr_btidx()), end_27 = gp0_1 - 3 ; i_13 <= end_27 ; i_13 = i_13 + (hmpprt::gr_btnumx()))
   {
    # 14 "<preprocessor>"
    hmpprt::s32 m_9;
    # 307 "y_solve.c"
    # 307 "y_solve.c"
    for (m_9 = 0 ; m_9 <= 2 ; m_9 = m_9 + 1)
    {
     }
    # 5 "<preprocessor>"
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
void __hmpp_acc_region__y_solve_64__mhml6p0i_internal_1(hmpprt::s32 ni, hmpprt::s32 nj, hmpprt::s32 gp0, hmpprt::s32 gp1_2, hmpprt::s32 gp2)
{
 # 5 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter((hmpprt::s32) (gp0), "gp0_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (gp1_2), "gp1_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (gp2), "gp2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ni), "ni_1");
  __hmppcg_call.launch(__hmpp_acc_region__y_solve_64__mhml6p0i_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__y_solve_64__mhml6p0i(hmpprt::s32 ni_2, hmpprt::s32 nj_1, hmpprt::s32 gp0_2, hmpprt::s32 gp1, hmpprt::s32 gp2_2)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__y_solve_64__mhml6p0i_internal_1(ni_2, nj_1, gp0_2, gp1, gp2_2));
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
      __hmpp_acc_region__y_solve_64__mhml6p0i_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__y_solve_64__mhml6p0i_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__y_solve_64__mhml6p0i", "prototype __hmpp_acc_region__y_solve_64__mhml6p0i(ni: s32, nj: s32, gp0: s32, gp1: s32, gp2: s32)");

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
      delete __hmpp_acc_region__y_solve_64__mhml6p0i_parallel_region_1;

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
