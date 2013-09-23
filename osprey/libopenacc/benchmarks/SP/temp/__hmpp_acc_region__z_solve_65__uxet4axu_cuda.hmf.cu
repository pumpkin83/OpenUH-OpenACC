
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
extern "C" CDLT_API  void __hmpp_acc_region__z_solve_65__uxet4axu(hmpprt::s32 nx2_4, hmpprt::s32 nz2_3, hmpprt::s32 nj_1, hmpprt::s32 ni_2, hmpprt::s32 gp2_2)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__z_solve_65__uxet4axu_internal_1(hmpprt::s32 nx2, hmpprt::s32 nz2, hmpprt::s32 nj, hmpprt::s32 ni, hmpprt::s32 gp2)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_4 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_4(hmpprt::s32 nx2_1, hmpprt::s32 ny2_3, hmpprt::s32 nz2_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_3 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_3(hmpprt::s32 gp2_1, hmpprt::s32 nx2_2, hmpprt::s32 ny2_2);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_2 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_2(hmpprt::s32 ni_1, hmpprt::s32 ny2_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_1(hmpprt::s32 nx2_3, hmpprt::s32 ny2_4, hmpprt::s32 nz2_2);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_1(hmpprt::s32 nx2_3, hmpprt::s32 ny2_4, hmpprt::s32 nz2_2)
{
 # 18 "<preprocessor>"
 hmpprt::s32 iter_per_gang_1;
 # 18 "<preprocessor>"
 hmpprt::s32 first_gang_iter_1;
 # 18 "<preprocessor>"
 hmpprt::s32 last_gang_iter_1;
 # 18 "<preprocessor>"
 iter_per_gang_1 = ((1 + (ny2_4 - 1) / 192) > 8LL ? (1 + (ny2_4 - 1) / 192) : 8LL);
 # 18 "<preprocessor>"
 first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
 # 18 "<preprocessor>"
 last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (ny2_4 - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (ny2_4 - 1));
 # 18 "<preprocessor>"
 hmpprt::s32 j_3;
 # 116 "z_solve.c"
 # 116 "z_solve.c"
 for (j_3 = first_gang_iter_1 + (hmpprt::gr_btidy()) ; j_3 <= last_gang_iter_1 ; j_3 = j_3 + (hmpprt::gr_btnumy()))
 {
  # 19 "<preprocessor>"
  hmpprt::s32 end_1;
  # 19 "<preprocessor>"
  hmpprt::s32 i_4;
  # 120 "z_solve.c"
  # 120 "z_solve.c"
  # 120 "z_solve.c"
  for (i_4 = (hmpprt::gr_btidx()), end_1 = nx2_3 - 1 ; i_4 <= end_1 ; i_4 = i_4 + (hmpprt::gr_btnumx()))
  {
   }
  # 17 "<preprocessor>"
  # 17 "<preprocessor>"
  hmpprt::s32 end_3;
  # 17 "<preprocessor>"
  hmpprt::s32 k_3;
  # 133 "z_solve.c"
  # 133 "z_solve.c"
  # 133 "z_solve.c"
  for (k_3 = (hmpprt::gr_btidx()), end_3 = nz2_2 - 5 ; k_3 <= end_3 ; k_3 = k_3 + (hmpprt::gr_btnumx()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_2;
   # 19 "<preprocessor>"
   hmpprt::s32 i_5;
   # 134 "z_solve.c"
   # 134 "z_solve.c"
   # 134 "z_solve.c"
   for (i_5 = 0, end_2 = nx2_3 - 1 ; i_5 <= end_2 ; i_5 = i_5 + 1)
   {
    }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_4;
  # 19 "<preprocessor>"
  hmpprt::s32 i_6;
  # 143 "z_solve.c"
  # 143 "z_solve.c"
  # 143 "z_solve.c"
  for (i_6 = (hmpprt::gr_btidx()), end_4 = nx2_3 - 1 ; i_6 <= end_4 ; i_6 = i_6 + (hmpprt::gr_btnumx()))
  {
   }
  # 17 "<preprocessor>"
  # 17 "<preprocessor>"
  hmpprt::s32 end_6;
  # 17 "<preprocessor>"
  hmpprt::s32 k_4;
  # 159 "z_solve.c"
  # 159 "z_solve.c"
  # 159 "z_solve.c"
  for (k_4 = (hmpprt::gr_btidx()), end_6 = nz2_2 - 1 ; k_4 <= end_6 ; k_4 = k_4 + (hmpprt::gr_btnumx()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_5;
   # 19 "<preprocessor>"
   hmpprt::s32 i_7;
   # 160 "z_solve.c"
   # 160 "z_solve.c"
   # 160 "z_solve.c"
   for (i_7 = 0, end_5 = nx2_3 - 1 ; i_7 <= end_5 ; i_7 = i_7 + 1)
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

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_2(hmpprt::s32 ni_1, hmpprt::s32 ny2_1)
{
 # 18 "<preprocessor>"
 hmpprt::s32 iter_per_gang_2;
 # 18 "<preprocessor>"
 hmpprt::s32 first_gang_iter_2;
 # 18 "<preprocessor>"
 hmpprt::s32 last_gang_iter_2;
 # 18 "<preprocessor>"
 iter_per_gang_2 = 1 + (ny2_1 - 1) / 192;
 # 18 "<preprocessor>"
 first_gang_iter_2 = (hmpprt::gr_gbidx()) * iter_per_gang_2;
 # 18 "<preprocessor>"
 last_gang_iter_2 = ((first_gang_iter_2 + iter_per_gang_2 - 1) < (ny2_1 - 1) ? (first_gang_iter_2 + iter_per_gang_2 - 1) : (ny2_1 - 1));
 # 18 "<preprocessor>"
 hmpprt::s32 j_1;
 # 67 "z_solve.c"
 # 67 "z_solve.c"
 for (j_1 = first_gang_iter_2 ; j_1 <= last_gang_iter_2 ; j_1 = j_1 + 1)
 {
  # 19 "<preprocessor>"
  hmpprt::s32 end_9;
  # 19 "<preprocessor>"
  hmpprt::s32 i_2;
  # 69 "z_solve.c"
  # 69 "z_solve.c"
  # 69 "z_solve.c"
  for (i_2 = (hmpprt::gr_btidy()), end_9 = ni_1 - 1 ; i_2 <= end_9 ; i_2 = i_2 + (hmpprt::gr_btnumy()))
  {
   # 14 "<preprocessor>"
   hmpprt::s32 m_1;
   # 70 "z_solve.c"
   # 70 "z_solve.c"
   for (m_1 = (hmpprt::gr_btidx()) ; m_1 <= 4 ; m_1 = m_1 + (hmpprt::gr_btnumx()))
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

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_3(hmpprt::s32 gp2_1, hmpprt::s32 nx2_2, hmpprt::s32 ny2_2)
{
 # 18 "<preprocessor>"
 hmpprt::s32 iter_per_gang_3;
 # 18 "<preprocessor>"
 hmpprt::s32 first_gang_iter_3;
 # 18 "<preprocessor>"
 hmpprt::s32 last_gang_iter_3;
 # 18 "<preprocessor>"
 iter_per_gang_3 = ((1 + (ny2_2 - 1) / 192) > 8LL ? (1 + (ny2_2 - 1) / 192) : 8LL);
 # 18 "<preprocessor>"
 first_gang_iter_3 = (hmpprt::gr_gbidx()) * iter_per_gang_3;
 # 18 "<preprocessor>"
 last_gang_iter_3 = ((first_gang_iter_3 + iter_per_gang_3 - 1) < (ny2_2 - 1) ? (first_gang_iter_3 + iter_per_gang_3 - 1) : (ny2_2 - 1));
 # 18 "<preprocessor>"
 hmpprt::s32 j_4;
 # 177 "z_solve.c"
 # 177 "z_solve.c"
 for (j_4 = first_gang_iter_3 + (hmpprt::gr_btidy()) ; j_4 <= last_gang_iter_3 ; j_4 = j_4 + (hmpprt::gr_btnumy()))
 {
  # 17 "<preprocessor>"
  hmpprt::s32 end_15;
  # 17 "<preprocessor>"
  hmpprt::s32 k_5;
  # 181 "z_solve.c"
  # 181 "z_solve.c"
  # 181 "z_solve.c"
  for (k_5 = (hmpprt::gr_btidx()), end_15 = gp2_1 - 3 ; k_5 <= end_15 ; k_5 = k_5 + (hmpprt::gr_btnumx()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_14;
   # 19 "<preprocessor>"
   hmpprt::s32 i_8;
   # 184 "z_solve.c"
   # 184 "z_solve.c"
   # 184 "z_solve.c"
   for (i_8 = 0, end_14 = nx2_2 - 1 ; i_8 <= end_14 ; i_8 = i_8 + 1)
   {
    # 14 "<preprocessor>"
    hmpprt::s32 m_2;
    # 188 "z_solve.c"
    # 188 "z_solve.c"
    for (m_2 = 0 ; m_2 <= 2 ; m_2 = m_2 + 1)
    {
     }
    # 14 "<preprocessor>"
    # 14 "<preprocessor>"
    hmpprt::s32 m_3;
    # 193 "z_solve.c"
    # 193 "z_solve.c"
    for (m_3 = 0 ; m_3 <= 2 ; m_3 = m_3 + 1)
    {
     }
    # 14 "<preprocessor>"
    # 14 "<preprocessor>"
    hmpprt::s32 m_4;
    # 198 "z_solve.c"
    # 198 "z_solve.c"
    for (m_4 = 0 ; m_4 <= 2 ; m_4 = m_4 + 1)
    {
     }
    # 19 "<preprocessor>"
   }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_19;
  # 19 "<preprocessor>"
  hmpprt::s32 i_9;
  # 211 "z_solve.c"
  # 211 "z_solve.c"
  # 211 "z_solve.c"
  for (i_9 = (hmpprt::gr_btidx()), end_19 = nx2_2 - 1 ; i_9 <= end_19 ; i_9 = i_9 + (hmpprt::gr_btnumx()))
  {
   # 14 "<preprocessor>"
   hmpprt::s32 m_5;
   # 215 "z_solve.c"
   # 215 "z_solve.c"
   for (m_5 = 0 ; m_5 <= 2 ; m_5 = m_5 + 1)
   {
    }
   # 14 "<preprocessor>"
   # 14 "<preprocessor>"
   hmpprt::s32 m_6;
   # 220 "z_solve.c"
   # 220 "z_solve.c"
   for (m_6 = 0 ; m_6 <= 2 ; m_6 = m_6 + 1)
   {
    }
   # 14 "<preprocessor>"
   # 14 "<preprocessor>"
   hmpprt::s32 m_7;
   # 228 "z_solve.c"
   # 228 "z_solve.c"
   for (m_7 = 0 ; m_7 <= 2 ; m_7 = m_7 + 1)
   {
    }
   # 17 "<preprocessor>"
  }
  # 17 "<preprocessor>"
  # 17 "<preprocessor>"
  hmpprt::s32 end_21;
  # 17 "<preprocessor>"
  hmpprt::s32 k_6;
  # 236 "z_solve.c"
  # 236 "z_solve.c"
  # 236 "z_solve.c"
  for (k_6 = (hmpprt::gr_btidx()), end_21 = gp2_1 - 3 ; k_6 <= end_21 ; k_6 = k_6 + (hmpprt::gr_btnumx()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_20;
   # 19 "<preprocessor>"
   hmpprt::s32 i_10;
   # 239 "z_solve.c"
   # 239 "z_solve.c"
   # 239 "z_solve.c"
   for (i_10 = 0, end_20 = nx2_2 - 1 ; i_10 <= end_20 ; i_10 = i_10 + 1)
   {
    }
   # 19 "<preprocessor>"
  }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_22;
  # 19 "<preprocessor>"
  hmpprt::s32 i_11;
  # 271 "z_solve.c"
  # 271 "z_solve.c"
  # 271 "z_solve.c"
  for (i_11 = (hmpprt::gr_btidx()), end_22 = nx2_2 - 1 ; i_11 <= end_22 ; i_11 = i_11 + (hmpprt::gr_btnumx()))
  {
   }
  # 19 "<preprocessor>"
  # 19 "<preprocessor>"
  hmpprt::s32 end_24;
  # 19 "<preprocessor>"
  hmpprt::s32 i_12;
  # 304 "z_solve.c"
  # 304 "z_solve.c"
  # 304 "z_solve.c"
  for (i_12 = (hmpprt::gr_btidx()), end_24 = nx2_2 - 1 ; i_12 <= end_24 ; i_12 = i_12 + (hmpprt::gr_btnumx()))
  {
   # 14 "<preprocessor>"
   hmpprt::s32 m_8;
   # 305 "z_solve.c"
   # 305 "z_solve.c"
   for (m_8 = 0 ; m_8 <= 2 ; m_8 = m_8 + 1)
   {
    }
   # 17 "<preprocessor>"
  }
  # 17 "<preprocessor>"
  # 17 "<preprocessor>"
  hmpprt::s32 end_27;
  # 17 "<preprocessor>"
  hmpprt::s32 k_7;
  # 321 "z_solve.c"
  # 321 "z_solve.c"
  # 321 "z_solve.c"
  for (k_7 = (hmpprt::gr_btidx()), end_27 = gp2_1 - 3 ; k_7 <= end_27 ; k_7 = k_7 + (hmpprt::gr_btnumx()))
  {
   # 19 "<preprocessor>"
   hmpprt::s32 end_26;
   # 19 "<preprocessor>"
   hmpprt::s32 i_13;
   # 324 "z_solve.c"
   # 324 "z_solve.c"
   # 324 "z_solve.c"
   for (i_13 = 0, end_26 = nx2_2 - 1 ; i_13 <= end_26 ; i_13 = i_13 + 1)
   {
    # 14 "<preprocessor>"
    hmpprt::s32 m_9;
    # 325 "z_solve.c"
    # 325 "z_solve.c"
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

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_4(hmpprt::s32 nx2_1, hmpprt::s32 ny2_3, hmpprt::s32 nz2_1)
{
 # 18 "<preprocessor>"
 hmpprt::s32 iter_per_gang_4;
 # 18 "<preprocessor>"
 hmpprt::s32 first_gang_iter_4;
 # 18 "<preprocessor>"
 hmpprt::s32 last_gang_iter_4;
 # 18 "<preprocessor>"
 iter_per_gang_4 = ((1 + (ny2_3 - 1) / 192) > 8LL ? (1 + (ny2_3 - 1) / 192) : 8LL);
 # 18 "<preprocessor>"
 first_gang_iter_4 = (hmpprt::gr_gbidx()) * iter_per_gang_4;
 # 18 "<preprocessor>"
 last_gang_iter_4 = ((first_gang_iter_4 + iter_per_gang_4 - 1) < (ny2_3 - 1) ? (first_gang_iter_4 + iter_per_gang_4 - 1) : (ny2_3 - 1));
 # 18 "<preprocessor>"
 hmpprt::s32 j_2;
 # 87 "z_solve.c"
 # 87 "z_solve.c"
 for (j_2 = first_gang_iter_4 + (hmpprt::gr_btidy()) ; j_2 <= last_gang_iter_4 ; j_2 = j_2 + (hmpprt::gr_btnumy()))
 {
  # 19 "<preprocessor>"
  hmpprt::s32 end_31;
  # 19 "<preprocessor>"
  hmpprt::s32 i_3;
  # 95 "z_solve.c"
  # 95 "z_solve.c"
  # 95 "z_solve.c"
  for (i_3 = (hmpprt::gr_btidx()), end_31 = nx2_1 - 1 ; i_3 <= end_31 ; i_3 = i_3 + (hmpprt::gr_btnumx()))
  {
   # 17 "<preprocessor>"
   hmpprt::s32 end_29;
   # 17 "<preprocessor>"
   hmpprt::s32 k_1;
   # 96 "z_solve.c"
   # 96 "z_solve.c"
   # 96 "z_solve.c"
   for (k_1 = 0, end_29 = nz2_1 + 1 ; k_1 <= end_29 ; k_1 = k_1 + 1)
   {
    }
   # 17 "<preprocessor>"
   # 17 "<preprocessor>"
   hmpprt::s32 end_30;
   # 17 "<preprocessor>"
   hmpprt::s32 k_2;
   # 102 "z_solve.c"
   # 102 "z_solve.c"
   # 102 "z_solve.c"
   for (k_2 = 0, end_30 = nz2_1 - 1 ; k_2 <= end_30 ; k_2 = k_2 + 1)
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
void __hmpp_acc_region__z_solve_65__uxet4axu_internal_1(hmpprt::s32 nx2, hmpprt::s32 nz2, hmpprt::s32 nj, hmpprt::s32 ni, hmpprt::s32 gp2)
{
 # 38 "<preprocessor>"
 hmpprt::s32 ny2;
 # 5 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ni), "ni_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2), "ny2_1");
  __hmppcg_call.launch(__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_2, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 5 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2), "nx2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2), "ny2_3");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2), "nz2_1");
  __hmppcg_call.launch(__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_4, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 5 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2), "nx2_3");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2), "ny2_4");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2), "nz2_2");
  __hmppcg_call.launch(__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
 # 5 "<preprocessor>"
 if (1)
 {
  hmpprt::CUDAGridCall __hmppcg_call;
  __hmppcg_call.setSizeX(192);
  __hmppcg_call.setSizeY(1);
  __hmppcg_call.setBlockSizeX(32);
  __hmppcg_call.setBlockSizeY(8LL);
  __hmppcg_call.addLocalParameter((hmpprt::s32) (gp2), "gp2_1");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2), "nx2_2");
  __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2), "ny2_2");
  __hmppcg_call.launch(__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_3, hmpprt::Context::getInstance()->getCUDADevice());
 }
 ;
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__z_solve_65__uxet4axu(hmpprt::s32 nx2_4, hmpprt::s32 nz2_3, hmpprt::s32 nj_1, hmpprt::s32 ni_2, hmpprt::s32 gp2_2)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__z_solve_65__uxet4axu_internal_1(nx2_4, nz2_3, nj_1, ni_2, gp2_2));
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
      __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_1");
      __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_2 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_2");
      __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_3 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_3");
      __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_4 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_4");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__z_solve_65__uxet4axu", "prototype __hmpp_acc_region__z_solve_65__uxet4axu(nx2: s32, nz2: s32, nj: s32, ni: s32, gp2: s32)");

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
      delete __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_1;
      delete __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_2;
      delete __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_3;
      delete __hmpp_acc_region__z_solve_65__uxet4axu_parallel_region_4;

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
