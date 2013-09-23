
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
extern "C" CDLT_API  void __hmpp_acc_region__x_solve_75__bbz1aha1(hmpprt::u64* __hmpp_vla_sizes__rhsX_2, hmpprt::s32 nx2_2, hmpprt::s32 nz2_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32 ni_2, hmpprt::s32 gp0_2, double* rhsX_2)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__x_solve_75__bbz1aha1_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64>  __hmpp_vla_sizes__rhsX_1, hmpprt::s32 nx2, hmpprt::s32 nz2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, hmpprt::s32 ni, hmpprt::s32 gp0_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhsX)
;
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
static hmpprt::CUDAGrid * __hmpp_acc_region__x_solve_75__bbz1aha1_parallel_region_1 = 0;
#else

extern "C" __global__ void __hmpp_acc_region__x_solve_75__bbz1aha1_parallel_region_1(double c1c5_1, double c2dttx1_1, double c3c4_1, double comz1_1, double comz4_1, double comz5_1, double comz6_1, double con43_1, double dttx1_1, double dttx2_1, double dx1_1, double dx2_1, double dx5_1, double dxmax_1, hmpprt::s32 gp0, hmpprt::s32* i_9, hmpprt::s32* j_13, double* lhsX_1, double* lhsmX_1, double* lhspX, hmpprt::s32 ni_1, hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, hmpprt::s32 nz2_1, double* rho_i_1, double* rhonX_1, hmpprt::u64* __hmpp_vla_sizes__rhsX, double* rhsX_1, double* speed_1, double* us_1);
#endif // __CUDACC__




# 5 "<preprocessor>"

#ifdef __CUDACC__

extern "C" __global__ void __hmpp_acc_region__x_solve_75__bbz1aha1_parallel_region_1(double c1c5_1, double c2dttx1_1, double c3c4_1, double comz1_1, double comz4_1, double comz5_1, double comz6_1, double con43_1, double dttx1_1, double dttx2_1, double dx1_1, double dx2_1, double dx5_1, double dxmax_1, hmpprt::s32 gp0, hmpprt::s32* i_9, hmpprt::s32* j_13, double* lhsX_1, double* lhsmX_1, double* lhspX, hmpprt::s32 ni_1, hmpprt::s32 nx2_1, hmpprt::s32 ny2_1, hmpprt::s32 nz2_1, double* rho_i_1, double* rhonX_1, hmpprt::u64* __hmpp_vla_sizes__rhsX, double* rhsX_1, double* speed_1, double* us_1)
{
 # 17 "<preprocessor>"
 hmpprt::s32 end_29;
 # 17 "<preprocessor>"
 hmpprt::s32 k_1;
 # 77 "x_solve.c"
 # 77 "x_solve.c"
 # 77 "x_solve.c"
 for (k_1 = 0, end_29 = nz2_1 - 1 ; k_1 <= end_29 ; k_1 = k_1 + 1)
 {
  # 18 "<preprocessor>"
  hmpprt::s32 iter_per_gang_1;
  # 18 "<preprocessor>"
  hmpprt::s32 first_gang_iter_1;
  # 18 "<preprocessor>"
  hmpprt::s32 last_gang_iter_1;
  # 18 "<preprocessor>"
  iter_per_gang_1 = ((1 + (ny2_1 - 1) / 192) > 8LL ? (1 + (ny2_1 - 1) / 192) : 8LL);
  # 18 "<preprocessor>"
  first_gang_iter_1 = (hmpprt::gr_gbidx()) * iter_per_gang_1;
  # 18 "<preprocessor>"
  last_gang_iter_1 = ((first_gang_iter_1 + iter_per_gang_1 - 1) < (ny2_1 - 1) ? (first_gang_iter_1 + iter_per_gang_1 - 1) : (ny2_1 - 1));
  # 18 "<preprocessor>"
  hmpprt::s32 j_1;
  # 79 "x_solve.c"
  # 79 "x_solve.c"
  for (j_1 = first_gang_iter_1 + (hmpprt::gr_btidy()) ; j_1 <= last_gang_iter_1 ; j_1 = j_1 + (hmpprt::gr_btnumy()))
  {
   # 14 "<preprocessor>"
   hmpprt::s32 m_1;
   # 80 "x_solve.c"
   # 80 "x_solve.c"
   for (m_1 = (hmpprt::gr_btidx()) ; m_1 <= 4 ; m_1 = m_1 + (hmpprt::gr_btnumx()))
   {
    # 81 "x_solve.c"
    *(lhsX_1 + (j_1 + 1) + 37uLL * (37uLL * (k_1 + 1 + 36uLL * m_1))) = (double) 0.0;
    # 82 "x_solve.c"
    *(lhspX + (j_1 + 1) + 37uLL * (37uLL * (k_1 + 1 + 36uLL * m_1))) = (double) 0.0;
    # 83 "x_solve.c"
    *(lhsmX_1 + (j_1 + 1) + 37uLL * (37uLL * (k_1 + 1 + 36uLL * m_1))) = (double) 0.0;
    # 84 "x_solve.c"
    *(lhsX_1 + (j_1 + 1) + 37uLL * (ni_1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) = (double) 0.0;
    # 85 "x_solve.c"
    *(lhspX + (j_1 + 1) + 37uLL * (ni_1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) = (double) 0.0;
    # 86 "x_solve.c"
    *(lhsmX_1 + (j_1 + 1) + 37uLL * (ni_1 + 37uLL * (k_1 + 1 + 36uLL * m_1))) = (double) 0.0;
   }
   # 88 "x_solve.c"
   # 88 "x_solve.c"
   *(lhsX_1 + (j_1 + 1) + 37uLL * (37uLL * (k_1 + 73))) = (double) 1.0;
   # 89 "x_solve.c"
   *(lhspX + (j_1 + 1) + 37uLL * (37uLL * (k_1 + 73))) = (double) 1.0;
   # 90 "x_solve.c"
   *(lhsmX_1 + (j_1 + 1) + 37uLL * (37uLL * (k_1 + 73))) = (double) 1.0;
   # 91 "x_solve.c"
   *(lhsX_1 + (j_1 + 1) + 37uLL * (ni_1 + 37uLL * (k_1 + 73))) = (double) 1.0;
   # 92 "x_solve.c"
   *(lhspX + (j_1 + 1) + 37uLL * (ni_1 + 37uLL * (k_1 + 73))) = (double) 1.0;
   # 93 "x_solve.c"
   *(lhsmX_1 + (j_1 + 1) + 37uLL * (ni_1 + 37uLL * (k_1 + 73))) = (double) 1.0;
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_5;
  # 18 "<preprocessor>"
  hmpprt::s32 j_2;
  # 105 "x_solve.c"
  # 105 "x_solve.c"
  # 105 "x_solve.c"
  for (j_2 = 0, end_5 = ny2_1 - 1 ; j_2 <= end_5 ; j_2 = j_2 + 1)
  {
   # 19 "<preprocessor>"
   hmpprt::s32 iter_per_gang_2;
   # 19 "<preprocessor>"
   hmpprt::s32 first_gang_iter_2;
   # 19 "<preprocessor>"
   hmpprt::s32 last_gang_iter_2;
   # 19 "<preprocessor>"
   iter_per_gang_2 = ((1 + (gp0 - 1) / 192) > 8LL ? (1 + (gp0 - 1) / 192) : 8LL);
   # 19 "<preprocessor>"
   first_gang_iter_2 = (hmpprt::gr_gbidx()) * iter_per_gang_2;
   # 19 "<preprocessor>"
   last_gang_iter_2 = ((first_gang_iter_2 + iter_per_gang_2 - 1) < (gp0 - 1) ? (first_gang_iter_2 + iter_per_gang_2 - 1) : (gp0 - 1));
   # 19 "<preprocessor>"
   hmpprt::s32 i_2;
   # 106 "x_solve.c"
   # 106 "x_solve.c"
   for (i_2 = first_gang_iter_2 + (hmpprt::gr_btidy()) ; i_2 <= last_gang_iter_2 ; i_2 = i_2 + (hmpprt::gr_btnumy()))
   {
    # 107 "x_solve.c"
    double ru1_1;
    # 107 "x_solve.c"
    ru1_1 = c3c4_1 * *(rho_i_1 + i_2 + 37uLL * (j_2 + 1 + 37uLL * (k_1 + 1)));
    # 109 "x_solve.c"
    *(rhonX_1 + (j_2 + 1) + 37uLL * (i_2 + 36uLL * (k_1 + 1))) = ( (( (dx2_1 + con43_1 * ru1_1 > dx5_1 + c1c5_1 * ru1_1) ? (dx2_1 + con43_1 * ru1_1) : (dx5_1 + c1c5_1 * ru1_1)) > ( (dxmax_1 + ru1_1 > dx1_1) ? (dxmax_1 + ru1_1) : dx1_1)) ? ( (dx2_1 + con43_1 * ru1_1 > dx5_1 + c1c5_1 * ru1_1) ? (dx2_1 + con43_1 * ru1_1) : (dx5_1 + c1c5_1 * ru1_1)) : ( (dxmax_1 + ru1_1 > dx1_1) ? (dxmax_1 + ru1_1) : dx1_1));
   }
   # 18 "<preprocessor>"
   # 18 "<preprocessor>"
   if (1 <= nx2_1)
   {
    # 19 "<preprocessor>"
    hmpprt::s32 iter_per_gang_3;
    # 19 "<preprocessor>"
    hmpprt::s32 first_gang_iter_3;
    # 19 "<preprocessor>"
    hmpprt::s32 last_gang_iter_3;
    # 19 "<preprocessor>"
    hmpprt::s32 tmp_30;
    # 19 "<preprocessor>"
    iter_per_gang_3 = ((1 + (nx2_1 - 1) / 192) > 8LL ? (1 + (nx2_1 - 1) / 192) : 8LL);
    # 19 "<preprocessor>"
    first_gang_iter_3 = (hmpprt::gr_gbidx()) * iter_per_gang_3;
    # 19 "<preprocessor>"
    last_gang_iter_3 = ((first_gang_iter_3 + iter_per_gang_3 - 1) < (nx2_1 - 1) ? (first_gang_iter_3 + iter_per_gang_3 - 1) : (nx2_1 - 1));
    # 19 "<preprocessor>"
    hmpprt::s32 i_3;
    # 112 "x_solve.c"
    # 112 "x_solve.c"
    for (i_3 = first_gang_iter_3 + (hmpprt::gr_btidy()) ; i_3 <= last_gang_iter_3 ; i_3 = i_3 + (hmpprt::gr_btnumy()))
    {
     # 113 "x_solve.c"
     *(lhsX_1 + (j_2 + 1) + 37uLL * (i_3 + 1 + 37uLL * (k_1 + 1))) = (double) 0.0;
     # 117 "x_solve.c"
     *(lhsX_1 + (j_2 + 1) + 37uLL * (i_3 + 1 + 37uLL * (k_1 + 37))) =  - dttx2_1 * *(us_1 + i_3 + 37uLL * (j_2 + 1 + 37uLL * (k_1 + 1))) - dttx1_1 * *(rhonX_1 + (j_2 + 1) + 37uLL * (i_3 + 36uLL * (k_1 + 1)));
     # 118 "x_solve.c"
     *(lhsX_1 + (j_2 + 1) + 37uLL * (i_3 + 1 + 37uLL * (k_1 + 73))) = (double) 1.0 + c2dttx1_1 * *(rhonX_1 + (j_2 + 1) + 37uLL * (i_3 + 1 + 36uLL * (k_1 + 1)));
     # 119 "x_solve.c"
     *(lhsX_1 + (j_2 + 1) + 37uLL * (i_3 + 1 + 37uLL * (k_1 + 109))) = dttx2_1 * *(us_1 + (i_3 + 2) + 37uLL * (j_2 + 1 + 37uLL * (k_1 + 1))) - dttx1_1 * *(rhonX_1 + (j_2 + 1) + 37uLL * (i_3 + 2 + 36uLL * (k_1 + 1)));
     # 120 "x_solve.c"
     *(lhsX_1 + (j_2 + 1) + 37uLL * (i_3 + 1 + 37uLL * (k_1 + 145))) = (double) 0.0;
    }
    # 18 "<preprocessor>"
    # 18 "<preprocessor>"
    tmp_30 = nx2_1 + 1;
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = tmp_30;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
   else
   {
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = 1;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_6;
  # 18 "<preprocessor>"
  hmpprt::s32 j_3;
  # 127 "x_solve.c"
  # 127 "x_solve.c"
  # 127 "x_solve.c"
  for (j_3 = 0, end_6 = ny2_1 - 1 ; j_3 <= end_6 ; j_3 = j_3 + 1)
  {
   # 18 "<preprocessor>"
   double tmp_33;
   # 18 "<preprocessor>"
   double tmp_34;
   # 18 "<preprocessor>"
   double tmp_35;
   # 18 "<preprocessor>"
   double tmp_36;
   # 18 "<preprocessor>"
   double tmp_37;
   # 18 "<preprocessor>"
   double tmp_38;
   # 18 "<preprocessor>"
   double tmp_39;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *i_9 = 1;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_33 = *(lhsX_1 + (j_3 + 1) + 37uLL * (1 + 37uLL * (k_1 + 73))) + comz5_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_3 + 1) + 37uLL * (1 + 37uLL * (k_1 + 73))) = tmp_33;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_34 = *(lhsX_1 + (j_3 + 1) + 37uLL * (1 + 37uLL * (k_1 + 109))) - comz4_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_3 + 1) + 37uLL * (1 + 37uLL * (k_1 + 109))) = tmp_34;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_35 = *(lhsX_1 + (j_3 + 1) + 37uLL * (1 + 37uLL * (k_1 + 145))) + comz1_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_3 + 1) + 37uLL * (1 + 37uLL * (k_1 + 145))) = tmp_35;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_36 = *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 37))) - comz4_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 37))) = tmp_36;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_37 = *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 73))) + comz6_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 73))) = tmp_37;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_38 = *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 109))) - comz4_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 109))) = tmp_38;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_39 = *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 145))) + comz1_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_3 + 1) + 37uLL * (2 + 37uLL * (k_1 + 145))) = tmp_39;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_8;
  # 18 "<preprocessor>"
  hmpprt::s32 j_4;
  # 139 "x_solve.c"
  # 139 "x_solve.c"
  # 139 "x_solve.c"
  for (j_4 = 0, end_8 = ny2_1 - 1 ; j_4 <= end_8 ; j_4 = j_4 + 1)
  {
   # 18 "<preprocessor>"
   if (3 <= gp0 - 4)
   {
    # 19 "<preprocessor>"
    hmpprt::s32 iter_per_gang_4;
    # 19 "<preprocessor>"
    hmpprt::s32 first_gang_iter_4;
    # 19 "<preprocessor>"
    hmpprt::s32 last_gang_iter_4;
    # 19 "<preprocessor>"
    hmpprt::s32 tmp_40;
    # 19 "<preprocessor>"
    iter_per_gang_4 = ((1 + (gp0 - 7) / 192) > 8LL ? (1 + (gp0 - 7) / 192) : 8LL);
    # 19 "<preprocessor>"
    first_gang_iter_4 = (hmpprt::gr_gbidx()) * iter_per_gang_4;
    # 19 "<preprocessor>"
    last_gang_iter_4 = ((first_gang_iter_4 + iter_per_gang_4 - 1) < (gp0 - 7) ? (first_gang_iter_4 + iter_per_gang_4 - 1) : (gp0 - 7));
    # 19 "<preprocessor>"
    hmpprt::s32 i_4;
    # 140 "x_solve.c"
    # 140 "x_solve.c"
    for (i_4 = first_gang_iter_4 + (hmpprt::gr_btidy()) ; i_4 <= last_gang_iter_4 ; i_4 = i_4 + (hmpprt::gr_btnumy()))
    {
     # 141 "x_solve.c"
     *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 1))) = *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 1))) + comz1_1;
     # 142 "x_solve.c"
     *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 37))) = *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 37))) - comz4_1;
     # 143 "x_solve.c"
     *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 73))) = *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 73))) + comz6_1;
     # 144 "x_solve.c"
     *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 109))) = *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 109))) - comz4_1;
     # 145 "x_solve.c"
     *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 145))) = *(lhsX_1 + (j_4 + 1) + 37uLL * (i_4 + 3 + 37uLL * (k_1 + 145))) + comz1_1;
    }
    # 18 "<preprocessor>"
    # 18 "<preprocessor>"
    tmp_40 = gp0 +  -3;
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = tmp_40;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
   else
   {
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = 3;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_9;
  # 18 "<preprocessor>"
  hmpprt::s32 j_5;
  # 149 "x_solve.c"
  # 149 "x_solve.c"
  # 149 "x_solve.c"
  for (j_5 = 0, end_9 = ny2_1 - 1 ; j_5 <= end_9 ; j_5 = j_5 + 1)
  {
   # 18 "<preprocessor>"
   hmpprt::s32 tmp_42;
   # 18 "<preprocessor>"
   double tmp_43;
   # 18 "<preprocessor>"
   double tmp_44;
   # 18 "<preprocessor>"
   double tmp_45;
   # 18 "<preprocessor>"
   double tmp_46;
   # 18 "<preprocessor>"
   double tmp_47;
   # 18 "<preprocessor>"
   double tmp_48;
   # 18 "<preprocessor>"
   double tmp_49;
   # 18 "<preprocessor>"
   tmp_42 = gp0 - 3;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *i_9 = tmp_42;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_43 = *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 1))) + comz1_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 1))) = tmp_43;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_44 = *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 37))) - comz4_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 37))) = tmp_44;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_45 = *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 73))) + comz6_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 73))) = tmp_45;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_46 = *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) - comz4_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) = tmp_46;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_47 = *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 1 + 37uLL * (k_1 + 1))) + comz1_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 1 + 37uLL * (k_1 + 1))) = tmp_47;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_48 = *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 1 + 37uLL * (k_1 + 37))) - comz4_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 1 + 37uLL * (k_1 + 37))) = tmp_48;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_49 = *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 1 + 37uLL * (k_1 + 73))) + comz5_1;
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsX_1 + (j_5 + 1) + 37uLL * (*i_9 + 1 + 37uLL * (k_1 + 73))) = tmp_49;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_11;
  # 18 "<preprocessor>"
  hmpprt::s32 j_6;
  # 165 "x_solve.c"
  # 165 "x_solve.c"
  # 165 "x_solve.c"
  for (j_6 = 0, end_11 = ny2_1 - 1 ; j_6 <= end_11 ; j_6 = j_6 + 1)
  {
   # 18 "<preprocessor>"
   if (1 <= nx2_1)
   {
    # 19 "<preprocessor>"
    hmpprt::s32 iter_per_gang_5;
    # 19 "<preprocessor>"
    hmpprt::s32 first_gang_iter_5;
    # 19 "<preprocessor>"
    hmpprt::s32 last_gang_iter_5;
    # 19 "<preprocessor>"
    hmpprt::s32 tmp_50;
    # 19 "<preprocessor>"
    iter_per_gang_5 = ((1 + (nx2_1 - 1) / 192) > 8LL ? (1 + (nx2_1 - 1) / 192) : 8LL);
    # 19 "<preprocessor>"
    first_gang_iter_5 = (hmpprt::gr_gbidx()) * iter_per_gang_5;
    # 19 "<preprocessor>"
    last_gang_iter_5 = ((first_gang_iter_5 + iter_per_gang_5 - 1) < (nx2_1 - 1) ? (first_gang_iter_5 + iter_per_gang_5 - 1) : (nx2_1 - 1));
    # 19 "<preprocessor>"
    hmpprt::s32 i_5;
    # 166 "x_solve.c"
    # 166 "x_solve.c"
    for (i_5 = first_gang_iter_5 + (hmpprt::gr_btidy()) ; i_5 <= last_gang_iter_5 ; i_5 = i_5 + (hmpprt::gr_btnumy()))
    {
     # 167 "x_solve.c"
     *(lhspX + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 1))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 1)));
     # 168 "x_solve.c"
     *(lhspX + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 37))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 37))) - dttx2_1 * *(speed_1 + i_5 + 37uLL * (j_6 + 1 + 37uLL * (k_1 + 1)));
     # 169 "x_solve.c"
     *(lhspX + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 73))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 73)));
     # 170 "x_solve.c"
     *(lhspX + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 109))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 109))) + dttx2_1 * *(speed_1 + (i_5 + 2) + 37uLL * (j_6 + 1 + 37uLL * (k_1 + 1)));
     # 171 "x_solve.c"
     *(lhspX + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 145))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 145)));
     # 172 "x_solve.c"
     *(lhsmX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 1))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 1)));
     # 173 "x_solve.c"
     *(lhsmX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 37))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 37))) + dttx2_1 * *(speed_1 + i_5 + 37uLL * (j_6 + 1 + 37uLL * (k_1 + 1)));
     # 174 "x_solve.c"
     *(lhsmX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 73))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 73)));
     # 175 "x_solve.c"
     *(lhsmX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 109))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 109))) - dttx2_1 * *(speed_1 + (i_5 + 2) + 37uLL * (j_6 + 1 + 37uLL * (k_1 + 1)));
     # 176 "x_solve.c"
     *(lhsmX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 145))) = *(lhsX_1 + (j_6 + 1) + 37uLL * (i_5 + 1 + 37uLL * (k_1 + 145)));
    }
    # 18 "<preprocessor>"
    # 18 "<preprocessor>"
    tmp_50 = nx2_1 + 1;
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = tmp_50;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
   else
   {
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = 1;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_16;
  # 18 "<preprocessor>"
  hmpprt::s32 j_7;
  # 187 "x_solve.c"
  # 187 "x_solve.c"
  # 187 "x_solve.c"
  for (j_7 = 0, end_16 = ny2_1 - 1 ; j_7 <= end_16 ; j_7 = j_7 + 1)
  {
   # 18 "<preprocessor>"
   if (0 <= gp0 - 3)
   {
    # 19 "<preprocessor>"
    hmpprt::s32 tmp_58;
    # 19 "<preprocessor>"
    hmpprt::s32 end_15;
    # 19 "<preprocessor>"
    hmpprt::s32 i_6;
    # 188 "x_solve.c"
    # 188 "x_solve.c"
    # 188 "x_solve.c"
    for (i_6 = 0, end_15 = gp0 - 3 ; i_6 <= end_15 ; i_6 = i_6 + 1)
    {
     # 189 "x_solve.c"
     double fac1_1;
     # 189 "x_solve.c"
     hmpprt::s32 i1_1;
     # 189 "x_solve.c"
     hmpprt::s32 i2_2;
     # 189 "x_solve.c"
     hmpprt::s32 first_gang_iter_6;
     # 189 "x_solve.c"
     hmpprt::s32 last_gang_iter_6;
     # 189 "x_solve.c"
     hmpprt::s32 first_gang_iter_7;
     # 189 "x_solve.c"
     hmpprt::s32 last_gang_iter_7;
     # 189 "x_solve.c"
     hmpprt::s32 first_gang_iter_8;
     # 189 "x_solve.c"
     hmpprt::s32 last_gang_iter_8;
     # 189 "x_solve.c"
     double tmp_52;
     # 189 "x_solve.c"
     double tmp_53;
     # 189 "x_solve.c"
     double tmp_54;
     # 189 "x_solve.c"
     double tmp_55;
     # 189 "x_solve.c"
     double tmp_56;
     # 189 "x_solve.c"
     double tmp_57;
     # 189 "x_solve.c"
     i1_1 = i_6 + 1;
     # 190 "x_solve.c"
     i2_2 = i_6 + 2;
     # 191 "x_solve.c"
     fac1_1 = (double) 1.0 / *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 73)));
     # 14 "<preprocessor>"
     tmp_52 = fac1_1 * *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 109)));
     # 14 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 14 "<preprocessor>"
      *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 109))) = tmp_52;
     }
     # 14 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 14 "<preprocessor>"
     tmp_53 = fac1_1 * *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 145)));
     # 14 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 14 "<preprocessor>"
      *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 145))) = tmp_53;
     }
     # 14 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 14 "<preprocessor>"
     first_gang_iter_6 = (hmpprt::gr_gbidx()) * 8;
     # 14 "<preprocessor>"
     last_gang_iter_6 = ((first_gang_iter_6 + 7) < 2 ? (first_gang_iter_6 + 7) : 2);
     # 14 "<preprocessor>"
     hmpprt::s32 m_2;
     # 194 "x_solve.c"
     # 194 "x_solve.c"
     for (m_2 = first_gang_iter_6 + (hmpprt::gr_btidy()) ; m_2 <= last_gang_iter_6 ; m_2 = m_2 + (hmpprt::gr_btnumy()))
     {
      # 195 "x_solve.c"
      *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_6 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_2))) = fac1_1 * *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_6 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_2)));
     }
     # 14 "<preprocessor>"
     # 14 "<preprocessor>"
     tmp_54 = *(lhsX_1 + (j_7 + 1) + 37uLL * (i1_1 + 37uLL * (k_1 + 73))) - *(lhsX_1 + (j_7 + 1) + 37uLL * (i1_1 + 37uLL * (k_1 + 37))) * *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 109)));
     # 14 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 14 "<preprocessor>"
      *(lhsX_1 + (j_7 + 1) + 37uLL * (i1_1 + 37uLL * (k_1 + 73))) = tmp_54;
     }
     # 14 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 14 "<preprocessor>"
     tmp_55 = *(lhsX_1 + (j_7 + 1) + 37uLL * (i1_1 + 37uLL * (k_1 + 109))) - *(lhsX_1 + (j_7 + 1) + 37uLL * (i1_1 + 37uLL * (k_1 + 37))) * *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 145)));
     # 14 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 14 "<preprocessor>"
      *(lhsX_1 + (j_7 + 1) + 37uLL * (i1_1 + 37uLL * (k_1 + 109))) = tmp_55;
     }
     # 14 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 14 "<preprocessor>"
     first_gang_iter_7 = (hmpprt::gr_gbidx()) * 8;
     # 14 "<preprocessor>"
     last_gang_iter_7 = ((first_gang_iter_7 + 7) < 2 ? (first_gang_iter_7 + 7) : 2);
     # 14 "<preprocessor>"
     hmpprt::s32 m_3;
     # 199 "x_solve.c"
     # 199 "x_solve.c"
     for (m_3 = first_gang_iter_7 + (hmpprt::gr_btidy()) ; m_3 <= last_gang_iter_7 ; m_3 = m_3 + (hmpprt::gr_btnumy()))
     {
      # 200 "x_solve.c"
      *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_1 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_3))) = *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_1 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_3))) - *(lhsX_1 + (j_7 + 1) + 37uLL * (i1_1 + 37uLL * (k_1 + 37))) * *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_6 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_3)));
     }
     # 14 "<preprocessor>"
     # 14 "<preprocessor>"
     tmp_56 = *(lhsX_1 + (j_7 + 1) + 37uLL * (i2_2 + 37uLL * (k_1 + 37))) - *(lhsX_1 + (j_7 + 1) + 37uLL * (i2_2 + 37uLL * (k_1 + 1))) * *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 109)));
     # 14 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 14 "<preprocessor>"
      *(lhsX_1 + (j_7 + 1) + 37uLL * (i2_2 + 37uLL * (k_1 + 37))) = tmp_56;
     }
     # 14 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 14 "<preprocessor>"
     tmp_57 = *(lhsX_1 + (j_7 + 1) + 37uLL * (i2_2 + 37uLL * (k_1 + 73))) - *(lhsX_1 + (j_7 + 1) + 37uLL * (i2_2 + 37uLL * (k_1 + 1))) * *(lhsX_1 + (j_7 + 1) + 37uLL * (i_6 + 37uLL * (k_1 + 145)));
     # 14 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 14 "<preprocessor>"
      *(lhsX_1 + (j_7 + 1) + 37uLL * (i2_2 + 37uLL * (k_1 + 73))) = tmp_57;
     }
     # 14 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 14 "<preprocessor>"
     first_gang_iter_8 = (hmpprt::gr_gbidx()) * 8;
     # 14 "<preprocessor>"
     last_gang_iter_8 = ((first_gang_iter_8 + 7) < 2 ? (first_gang_iter_8 + 7) : 2);
     # 14 "<preprocessor>"
     hmpprt::s32 m_4;
     # 204 "x_solve.c"
     # 204 "x_solve.c"
     for (m_4 = first_gang_iter_8 + (hmpprt::gr_btidy()) ; m_4 <= last_gang_iter_8 ; m_4 = m_4 + (hmpprt::gr_btnumy()))
     {
      # 205 "x_solve.c"
      *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_4))) = *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_4))) - *(lhsX_1 + (j_7 + 1) + 37uLL * (i2_2 + 37uLL * (k_1 + 1))) * *(rhsX_1 + (j_7 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_6 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_4)));
     }
     # 18 "<preprocessor>"
    }
    # 18 "<preprocessor>"
    # 18 "<preprocessor>"
    tmp_58 = gp0 +  -2;
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = tmp_58;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
   else
   {
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = 0;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_20;
  # 18 "<preprocessor>"
  hmpprt::s32 j_8;
  # 215 "x_solve.c"
  # 215 "x_solve.c"
  # 215 "x_solve.c"
  for (j_8 = 0, end_20 = ny2_1 - 1 ; j_8 <= end_20 ; j_8 = j_8 + 1)
  {
   # 217 "x_solve.c"
   double fac1_2;
   # 217 "x_solve.c"
   hmpprt::s32 i1_3;
   # 217 "x_solve.c"
   double fac2_1;
   # 217 "x_solve.c"
   hmpprt::s32 first_gang_iter_9;
   # 217 "x_solve.c"
   hmpprt::s32 last_gang_iter_9;
   # 217 "x_solve.c"
   hmpprt::s32 first_gang_iter_10;
   # 217 "x_solve.c"
   hmpprt::s32 last_gang_iter_10;
   # 217 "x_solve.c"
   hmpprt::s32 first_gang_iter_11;
   # 217 "x_solve.c"
   hmpprt::s32 last_gang_iter_11;
   # 217 "x_solve.c"
   hmpprt::s32 tmp_60;
   # 217 "x_solve.c"
   double tmp_61;
   # 217 "x_solve.c"
   double tmp_62;
   # 217 "x_solve.c"
   double tmp_63;
   # 217 "x_solve.c"
   double tmp_64;
   # 217 "x_solve.c"
   tmp_60 = gp0 - 2;
   # 217 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 217 "x_solve.c"
    *i_9 = tmp_60;
   }
   # 217 "x_solve.c"
   (hmpprt::gr_barrier());
   # 217 "x_solve.c"
   i1_3 = gp0 - 1;
   # 218 "x_solve.c"
   fac1_2 = (double) 1.0 / *(lhsX_1 + (j_8 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 73)));
   # 14 "<preprocessor>"
   tmp_61 = fac1_2 * *(lhsX_1 + (j_8 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109)));
   # 14 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 14 "<preprocessor>"
    *(lhsX_1 + (j_8 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) = tmp_61;
   }
   # 14 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 14 "<preprocessor>"
   tmp_62 = fac1_2 * *(lhsX_1 + (j_8 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145)));
   # 14 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 14 "<preprocessor>"
    *(lhsX_1 + (j_8 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145))) = tmp_62;
   }
   # 14 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 14 "<preprocessor>"
   first_gang_iter_9 = (hmpprt::gr_gbidx()) * 8;
   # 14 "<preprocessor>"
   last_gang_iter_9 = ((first_gang_iter_9 + 7) < 2 ? (first_gang_iter_9 + 7) : 2);
   # 14 "<preprocessor>"
   hmpprt::s32 m_5;
   # 221 "x_solve.c"
   # 221 "x_solve.c"
   for (m_5 = first_gang_iter_9 + (hmpprt::gr_btidy()) ; m_5 <= last_gang_iter_9 ; m_5 = m_5 + (hmpprt::gr_btnumy()))
   {
    # 222 "x_solve.c"
    *(rhsX_1 + (j_8 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_5))) = fac1_2 * *(rhsX_1 + (j_8 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_5)));
   }
   # 14 "<preprocessor>"
   # 14 "<preprocessor>"
   tmp_63 = *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 73))) - *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 37))) * *(lhsX_1 + (j_8 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109)));
   # 14 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 14 "<preprocessor>"
    *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 73))) = tmp_63;
   }
   # 14 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 14 "<preprocessor>"
   tmp_64 = *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 109))) - *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 37))) * *(lhsX_1 + (j_8 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145)));
   # 14 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 14 "<preprocessor>"
    *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 109))) = tmp_64;
   }
   # 14 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 14 "<preprocessor>"
   first_gang_iter_10 = (hmpprt::gr_gbidx()) * 8;
   # 14 "<preprocessor>"
   last_gang_iter_10 = ((first_gang_iter_10 + 7) < 2 ? (first_gang_iter_10 + 7) : 2);
   # 14 "<preprocessor>"
   hmpprt::s32 m_6;
   # 226 "x_solve.c"
   # 226 "x_solve.c"
   for (m_6 = first_gang_iter_10 + (hmpprt::gr_btidy()) ; m_6 <= last_gang_iter_10 ; m_6 = m_6 + (hmpprt::gr_btnumy()))
   {
    # 227 "x_solve.c"
    *(rhsX_1 + (j_8 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_6))) = *(rhsX_1 + (j_8 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_6))) - *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 37))) * *(rhsX_1 + (j_8 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_6)));
   }
   # 233 "x_solve.c"
   # 233 "x_solve.c"
   fac2_1 = (double) 1.0 / *(lhsX_1 + (j_8 + 1) + 37uLL * (i1_3 + 37uLL * (k_1 + 73)));
   # 14 "<preprocessor>"
   first_gang_iter_11 = (hmpprt::gr_gbidx()) * 8;
   # 14 "<preprocessor>"
   last_gang_iter_11 = ((first_gang_iter_11 + 7) < 2 ? (first_gang_iter_11 + 7) : 2);
   # 14 "<preprocessor>"
   hmpprt::s32 m_7;
   # 234 "x_solve.c"
   # 234 "x_solve.c"
   for (m_7 = first_gang_iter_11 + (hmpprt::gr_btidy()) ; m_7 <= last_gang_iter_11 ; m_7 = m_7 + (hmpprt::gr_btnumy()))
   {
    # 235 "x_solve.c"
    *(rhsX_1 + (j_8 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_7))) = fac2_1 * *(rhsX_1 + (j_8 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_7)));
   }
   # 18 "<preprocessor>"
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_22;
  # 18 "<preprocessor>"
  hmpprt::s32 j_9;
  # 242 "x_solve.c"
  # 242 "x_solve.c"
  # 242 "x_solve.c"
  for (j_9 = 0, end_22 = ny2_1 - 1 ; j_9 <= end_22 ; j_9 = j_9 + 1)
  {
   # 18 "<preprocessor>"
   if (0 <= gp0 - 3)
   {
    # 19 "<preprocessor>"
    hmpprt::s32 tmp_83;
    # 19 "<preprocessor>"
    hmpprt::s32 end_21;
    # 19 "<preprocessor>"
    hmpprt::s32 i_7;
    # 243 "x_solve.c"
    # 243 "x_solve.c"
    # 243 "x_solve.c"
    for (i_7 = 0, end_21 = gp0 - 3 ; i_7 <= end_21 ; i_7 = i_7 + 1)
    {
     # 244 "x_solve.c"
     double fac1_3;
     # 244 "x_solve.c"
     hmpprt::s32 i1_4;
     # 244 "x_solve.c"
     hmpprt::s32 i2_3;
     # 244 "x_solve.c"
     double fac1_4;
     # 244 "x_solve.c"
     double tmp_65;
     # 244 "x_solve.c"
     double tmp_66;
     # 244 "x_solve.c"
     double tmp_67;
     # 244 "x_solve.c"
     double tmp_68;
     # 244 "x_solve.c"
     double tmp_69;
     # 244 "x_solve.c"
     double tmp_70;
     # 244 "x_solve.c"
     double tmp_71;
     # 244 "x_solve.c"
     double tmp_72;
     # 244 "x_solve.c"
     double tmp_73;
     # 244 "x_solve.c"
     double tmp_74;
     # 244 "x_solve.c"
     double tmp_75;
     # 244 "x_solve.c"
     double tmp_76;
     # 244 "x_solve.c"
     double tmp_77;
     # 244 "x_solve.c"
     double tmp_78;
     # 244 "x_solve.c"
     double tmp_79;
     # 244 "x_solve.c"
     double tmp_80;
     # 244 "x_solve.c"
     double tmp_81;
     # 244 "x_solve.c"
     double tmp_82;
     # 244 "x_solve.c"
     i1_4 = i_7 + 1;
     # 245 "x_solve.c"
     i2_3 = i_7 + 2;
     # 248 "x_solve.c"
     fac1_3 = (double) 1.0 / *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 73)));
     # 260 "x_solve.c"
     tmp_65 = fac1_3 * *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109))) = tmp_65;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_66 = fac1_3 * *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145))) = tmp_66;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_67 = fac1_3 * *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_67;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_68 = *(lhspX + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 73))) - *(lhspX + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 37))) * *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(lhspX + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 73))) = tmp_68;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_69 = *(lhspX + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 109))) - *(lhspX + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 37))) * *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(lhspX + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 109))) = tmp_69;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_70 = *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_4 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) - *(lhspX + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 37))) * *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_4 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_70;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_71 = *(lhspX + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 37))) - *(lhspX + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 1))) * *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(lhspX + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 37))) = tmp_71;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_72 = *(lhspX + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 73))) - *(lhspX + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 1))) * *(lhspX + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(lhspX + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 73))) = tmp_72;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     tmp_73 = *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) - *(lhspX + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 1))) * *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
     # 260 "x_solve.c"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 260 "x_solve.c"
      *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_73;
     }
     # 260 "x_solve.c"
     (hmpprt::gr_barrier());
     # 260 "x_solve.c"
     fac1_4 = (double) 1.0 / *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 73)));
     # 18 "<preprocessor>"
     tmp_74 = fac1_4 * *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109))) = tmp_74;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_75 = fac1_4 * *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145))) = tmp_75;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_76 = fac1_4 * *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_76;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_77 = *(lhsmX_1 + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 73))) - *(lhsmX_1 + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 37))) * *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(lhsmX_1 + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 73))) = tmp_77;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_78 = *(lhsmX_1 + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 109))) - *(lhsmX_1 + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 37))) * *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(lhsmX_1 + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 109))) = tmp_78;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_79 = *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_4 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) - *(lhsmX_1 + (j_9 + 1) + 37uLL * (i1_4 + 37uLL * (k_1 + 37))) * *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_4 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_79;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_80 = *(lhsmX_1 + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 37))) - *(lhsmX_1 + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 1))) * *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 109)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(lhsmX_1 + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 37))) = tmp_80;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_81 = *(lhsmX_1 + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 73))) - *(lhsmX_1 + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 1))) * *(lhsmX_1 + (j_9 + 1) + 37uLL * (i_7 + 37uLL * (k_1 + 145)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(lhsmX_1 + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 73))) = tmp_81;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
     # 18 "<preprocessor>"
     tmp_82 = *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) - *(lhsmX_1 + (j_9 + 1) + 37uLL * (i2_3 + 37uLL * (k_1 + 1))) * *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i_7 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
     # 18 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 18 "<preprocessor>"
      *(rhsX_1 + (j_9 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_3 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_82;
     }
     # 18 "<preprocessor>"
     (hmpprt::gr_barrier());
    }
    # 18 "<preprocessor>"
    # 18 "<preprocessor>"
    tmp_83 = gp0 +  -2;
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = tmp_83;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
   else
   {
    # 18 "<preprocessor>"
    if ((hmpprt::gr_btidy()) == 0)
    {
     # 18 "<preprocessor>"
     *i_9 = 0;
    }
    # 18 "<preprocessor>"
    (hmpprt::gr_barrier());
   }
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_23;
  # 18 "<preprocessor>"
  hmpprt::s32 j_10;
  # 276 "x_solve.c"
  # 276 "x_solve.c"
  # 276 "x_solve.c"
  for (j_10 = 0, end_23 = ny2_1 - 1 ; j_10 <= end_23 ; j_10 = j_10 + 1)
  {
   # 278 "x_solve.c"
   double fac1_5;
   # 278 "x_solve.c"
   hmpprt::s32 i1_5;
   # 278 "x_solve.c"
   double fac1_6;
   # 278 "x_solve.c"
   hmpprt::s32 tmp_85;
   # 278 "x_solve.c"
   double tmp_86;
   # 278 "x_solve.c"
   double tmp_87;
   # 278 "x_solve.c"
   double tmp_88;
   # 278 "x_solve.c"
   double tmp_89;
   # 278 "x_solve.c"
   double tmp_90;
   # 278 "x_solve.c"
   double tmp_91;
   # 278 "x_solve.c"
   double tmp_92;
   # 278 "x_solve.c"
   double tmp_93;
   # 278 "x_solve.c"
   double tmp_94;
   # 278 "x_solve.c"
   double tmp_95;
   # 278 "x_solve.c"
   double tmp_96;
   # 278 "x_solve.c"
   double tmp_97;
   # 278 "x_solve.c"
   double tmp_98;
   # 278 "x_solve.c"
   double tmp_99;
   # 278 "x_solve.c"
   tmp_85 = gp0 - 2;
   # 278 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 278 "x_solve.c"
    *i_9 = tmp_85;
   }
   # 278 "x_solve.c"
   (hmpprt::gr_barrier());
   # 278 "x_solve.c"
   i1_5 = gp0 - 1;
   # 281 "x_solve.c"
   fac1_5 = (double) 1.0 / *(lhspX + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 73)));
   # 290 "x_solve.c"
   tmp_86 = fac1_5 * *(lhspX + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109)));
   # 290 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 290 "x_solve.c"
    *(lhspX + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) = tmp_86;
   }
   # 290 "x_solve.c"
   (hmpprt::gr_barrier());
   # 290 "x_solve.c"
   tmp_87 = fac1_5 * *(lhspX + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145)));
   # 290 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 290 "x_solve.c"
    *(lhspX + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145))) = tmp_87;
   }
   # 290 "x_solve.c"
   (hmpprt::gr_barrier());
   # 290 "x_solve.c"
   tmp_88 = fac1_5 * *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
   # 290 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 290 "x_solve.c"
    *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_88;
   }
   # 290 "x_solve.c"
   (hmpprt::gr_barrier());
   # 290 "x_solve.c"
   tmp_89 = *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 73))) - *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 37))) * *(lhspX + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109)));
   # 290 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 290 "x_solve.c"
    *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 73))) = tmp_89;
   }
   # 290 "x_solve.c"
   (hmpprt::gr_barrier());
   # 290 "x_solve.c"
   tmp_90 = *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 109))) - *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 37))) * *(lhspX + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145)));
   # 290 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 290 "x_solve.c"
    *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 109))) = tmp_90;
   }
   # 290 "x_solve.c"
   (hmpprt::gr_barrier());
   # 290 "x_solve.c"
   tmp_91 = *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) - *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 37))) * *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
   # 290 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 290 "x_solve.c"
    *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_91;
   }
   # 290 "x_solve.c"
   (hmpprt::gr_barrier());
   # 290 "x_solve.c"
   fac1_6 = (double) 1.0 / *(lhsmX_1 + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 73)));
   # 18 "<preprocessor>"
   tmp_92 = fac1_6 * *(lhsmX_1 + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsmX_1 + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) = tmp_92;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_93 = fac1_6 * *(lhsmX_1 + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsmX_1 + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145))) = tmp_93;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_94 = fac1_6 * *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_94;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_95 = *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 73))) - *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 37))) * *(lhsmX_1 + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 73))) = tmp_95;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_96 = *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 109))) - *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 37))) * *(lhsmX_1 + (j_10 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 145)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 109))) = tmp_96;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_97 = *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) - *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 37))) * *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_97;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_98 = *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) / *(lhspX + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 73)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_98;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 18 "<preprocessor>"
   tmp_99 = *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) / *(lhsmX_1 + (j_10 + 1) + 37uLL * (i1_5 + 37uLL * (k_1 + 73)));
   # 18 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 18 "<preprocessor>"
    *(rhsX_1 + (j_10 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_5 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_99;
   }
   # 18 "<preprocessor>"
   (hmpprt::gr_barrier());
  }
  # 18 "<preprocessor>"
  # 18 "<preprocessor>"
  hmpprt::s32 end_25;
  # 18 "<preprocessor>"
  hmpprt::s32 j_11;
  # 308 "x_solve.c"
  # 308 "x_solve.c"
  # 308 "x_solve.c"
  for (j_11 = 0, end_25 = ny2_1 - 1 ; j_11 <= end_25 ; j_11 = j_11 + 1)
  {
   # 310 "x_solve.c"
   hmpprt::s32 i1_6;
   # 310 "x_solve.c"
   hmpprt::s32 first_gang_iter_12;
   # 310 "x_solve.c"
   hmpprt::s32 last_gang_iter_12;
   # 310 "x_solve.c"
   hmpprt::s32 tmp_100;
   # 310 "x_solve.c"
   double tmp_101;
   # 310 "x_solve.c"
   double tmp_102;
   # 310 "x_solve.c"
   tmp_100 = gp0 - 2;
   # 310 "x_solve.c"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 310 "x_solve.c"
    *i_9 = tmp_100;
   }
   # 310 "x_solve.c"
   (hmpprt::gr_barrier());
   # 310 "x_solve.c"
   i1_6 = gp0 - 1;
   # 14 "<preprocessor>"
   first_gang_iter_12 = (hmpprt::gr_gbidx()) * 8;
   # 14 "<preprocessor>"
   last_gang_iter_12 = ((first_gang_iter_12 + 7) < 2 ? (first_gang_iter_12 + 7) : 2);
   # 14 "<preprocessor>"
   hmpprt::s32 m_8;
   # 311 "x_solve.c"
   # 311 "x_solve.c"
   for (m_8 = first_gang_iter_12 + (hmpprt::gr_btidy()) ; m_8 <= last_gang_iter_12 ; m_8 = m_8 + (hmpprt::gr_btnumy()))
   {
    # 312 "x_solve.c"
    *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_8))) = *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_8))) - *(lhsX_1 + (j_11 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) * *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_6 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_8)));
   }
   # 5 "<preprocessor>"
   # 5 "<preprocessor>"
   tmp_101 = *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) - *(lhspX + (j_11 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) * *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_6 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
   # 5 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 5 "<preprocessor>"
    *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_101;
   }
   # 5 "<preprocessor>"
   (hmpprt::gr_barrier());
   # 5 "<preprocessor>"
   tmp_102 = *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) - *(lhsmX_1 + (j_11 + 1) + 37uLL * (*i_9 + 37uLL * (k_1 + 109))) * *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_6 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
   # 5 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 5 "<preprocessor>"
    *(rhsX_1 + (j_11 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (*i_9 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_102;
   }
   # 5 "<preprocessor>"
   (hmpprt::gr_barrier());
  }
  # 5 "<preprocessor>"
  # 5 "<preprocessor>"
  if (1 <= ny2_1)
  {
   # 18 "<preprocessor>"
   hmpprt::s32 tmp_107;
   # 18 "<preprocessor>"
   hmpprt::s32 end_28;
   # 18 "<preprocessor>"
   hmpprt::s32 j_12;
   # 322 "x_solve.c"
   # 322 "x_solve.c"
   # 322 "x_solve.c"
   for (j_12 = 0, end_28 = ny2_1 - 1 ; j_12 <= end_28 ; j_12 = j_12 + 1)
   {
    # 5 "<preprocessor>"
    if (gp0 - 3 >= 0)
    {
     # 19 "<preprocessor>"
     hmpprt::s32 tmp_105;
     # 19 "<preprocessor>"
     hmpprt::s32 end_27;
     # 19 "<preprocessor>"
     hmpprt::s32 i_8;
     # 323 "x_solve.c"
     # 323 "x_solve.c"
     # 323 "x_solve.c"
     for (i_8 = 0, end_27 = gp0 - 3 ; i_8 <= end_27 ; i_8 = i_8 + 1)
     {
      # 324 "x_solve.c"
      hmpprt::s32 i2_1;
      # 324 "x_solve.c"
      hmpprt::s32 i1_2;
      # 324 "x_solve.c"
      hmpprt::s32 first_gang_iter_13;
      # 324 "x_solve.c"
      hmpprt::s32 last_gang_iter_13;
      # 324 "x_solve.c"
      double tmp_103;
      # 324 "x_solve.c"
      double tmp_104;
      # 324 "x_solve.c"
      i1_2 = gp0 - 3 - i_8 + 1;
      # 325 "x_solve.c"
      i2_1 = gp0 - 3 - i_8 + 2;
      # 14 "<preprocessor>"
      first_gang_iter_13 = (hmpprt::gr_gbidx()) * 8;
      # 14 "<preprocessor>"
      last_gang_iter_13 = ((first_gang_iter_13 + 7) < 2 ? (first_gang_iter_13 + 7) : 2);
      # 14 "<preprocessor>"
      hmpprt::s32 m_9;
      # 326 "x_solve.c"
      # 326 "x_solve.c"
      for (m_9 = first_gang_iter_13 + (hmpprt::gr_btidy()) ; m_9 <= last_gang_iter_13 ; m_9 = m_9 + (hmpprt::gr_btnumy()))
      {
       # 329 "x_solve.c"
       *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (gp0 - 3 - i_8 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_9))) = *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (gp0 - 3 - i_8 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_9))) - *(lhsX_1 + (j_12 + 1) + 37uLL * (gp0 - 3 - i_8 + 37uLL * (k_1 + 109))) * *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_9))) - *(lhsX_1 + (j_12 + 1) + 37uLL * (gp0 - 3 - i_8 + 37uLL * (k_1 + 145))) * *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_1 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * m_9)));
      }
      # 5 "<preprocessor>"
      # 5 "<preprocessor>"
      tmp_103 = *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (gp0 - 3 - i_8 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) - *(lhspX + (j_12 + 1) + 37uLL * (gp0 - 3 - i_8 + 37uLL * (k_1 + 109))) * *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) - *(lhspX + (j_12 + 1) + 37uLL * (gp0 - 3 - i_8 + 37uLL * (k_1 + 145))) * *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_1 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3)));
      # 5 "<preprocessor>"
      if ((hmpprt::gr_btidy()) == 0)
      {
       # 5 "<preprocessor>"
       *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (gp0 - 3 - i_8 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 3))) = tmp_103;
      }
      # 5 "<preprocessor>"
      (hmpprt::gr_barrier());
      # 5 "<preprocessor>"
      tmp_104 = *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (gp0 - 3 - i_8 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) - *(lhsmX_1 + (j_12 + 1) + 37uLL * (gp0 - 3 - i_8 + 37uLL * (k_1 + 109))) * *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i1_2 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) - *(lhsmX_1 + (j_12 + 1) + 37uLL * (gp0 - 3 - i_8 + 37uLL * (k_1 + 145))) * *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (i2_1 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4)));
      # 5 "<preprocessor>"
      if ((hmpprt::gr_btidy()) == 0)
      {
       # 5 "<preprocessor>"
       *(rhsX_1 + (j_12 + 1) + *(__hmpp_vla_sizes__rhsX + 3) * (gp0 - 3 - i_8 + *(__hmpp_vla_sizes__rhsX + 2) * (k_1 + 1 + *(__hmpp_vla_sizes__rhsX + 1) * 4))) = tmp_104;
      }
      # 5 "<preprocessor>"
      (hmpprt::gr_barrier());
     }
     # 5 "<preprocessor>"
     # 5 "<preprocessor>"
     tmp_105 = gp0 - 3 - (gp0 +  -2);
     # 5 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 5 "<preprocessor>"
      *i_9 = tmp_105;
     }
     # 5 "<preprocessor>"
     (hmpprt::gr_barrier());
    }
    else
    {
     # 5 "<preprocessor>"
     hmpprt::s32 tmp_106;
     # 5 "<preprocessor>"
     tmp_106 = gp0 - 3;
     # 5 "<preprocessor>"
     if ((hmpprt::gr_btidy()) == 0)
     {
      # 5 "<preprocessor>"
      *i_9 = tmp_106;
     }
     # 5 "<preprocessor>"
     (hmpprt::gr_barrier());
    }
   }
   # 5 "<preprocessor>"
   # 5 "<preprocessor>"
   tmp_107 = ny2_1 + 1;
   # 5 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 5 "<preprocessor>"
    *j_13 = tmp_107;
   }
   # 5 "<preprocessor>"
   (hmpprt::gr_barrier());
  }
  else
  {
   # 5 "<preprocessor>"
   if ((hmpprt::gr_btidy()) == 0)
   {
    # 5 "<preprocessor>"
    *j_13 = 1;
   }
   # 5 "<preprocessor>"
   (hmpprt::gr_barrier());
  }
 }
 # 5 "<preprocessor>"
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
void __hmpp_acc_region__x_solve_75__bbz1aha1_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64>  __hmpp_vla_sizes__rhsX_1, hmpprt::s32 nx2, hmpprt::s32 nz2, hmpprt::s32* __hmpp_addr__i, hmpprt::s32* __hmpp_addr__j, hmpprt::s32* __hmpp_addr__k, hmpprt::s32 ni, hmpprt::s32 gp0_1, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhsX)
{
 # 7 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rhonX;
 # 8 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&rhonX), hmpprt::MS_CUDA_GLOB, 383616uLL);
 # 8 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  lhsmX;
 # 9 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&lhsmX), hmpprt::MS_CUDA_GLOB, 1971360uLL);
 # 9 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  lhspX_1;
 # 10 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&lhspX_1), hmpprt::MS_CUDA_GLOB, 1971360uLL);
 # 10 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  lhsX;
 # 17 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&lhsX), hmpprt::MS_CUDA_GLOB, 1971360uLL);
 # 17 "<preprocessor>"
 hmpprt::s32 k;
 # 18 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  j;
 # 18 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&j), hmpprt::MS_CUDA_GLOB, 4);
 # 18 "<preprocessor>"
 *j = *__hmpp_addr__j;
 # 19 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::s32>  i_1;
 # 19 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&i_1), hmpprt::MS_CUDA_GLOB, 4);
 # 19 "<preprocessor>"
 *i_1 = *__hmpp_addr__i;
 # 20 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  speed;
 # 21 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&speed), hmpprt::MS_CUDA_GLOB, 394272uLL);
 # 21 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  rho_i;
 # 22 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&rho_i), hmpprt::MS_CUDA_GLOB, 394272uLL);
 # 22 "<preprocessor>"
 hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double>  us;
 # 23 "<preprocessor>"
 hmpprt::Context::getInstance()->allocate((void **) (&us), hmpprt::MS_CUDA_GLOB, 394272uLL);
 # 23 "<preprocessor>"
 double con43;
 # 24 "<preprocessor>"
 double comz6;
 # 25 "<preprocessor>"
 double comz5;
 # 26 "<preprocessor>"
 double comz4;
 # 27 "<preprocessor>"
 double comz1;
 # 28 "<preprocessor>"
 double c2dttx1;
 # 29 "<preprocessor>"
 double dttx2;
 # 30 "<preprocessor>"
 double dttx1;
 # 31 "<preprocessor>"
 double c3c4;
 # 32 "<preprocessor>"
 double c1c5;
 # 33 "<preprocessor>"
 double dxmax;
 # 34 "<preprocessor>"
 double dx5;
 # 35 "<preprocessor>"
 double dx2;
 # 36 "<preprocessor>"
 double dx1;
 # 37 "<preprocessor>"
 hmpprt::s32 ny2;
 # 312 "<preprocessor>"
 if (1 <= nz2)
 {
  # 312 "<preprocessor>"
  if (1)
  {
   hmpprt::CUDAGridCall __hmppcg_call;
   __hmppcg_call.setSizeX(192);
   __hmppcg_call.setSizeY(1);
   __hmppcg_call.setBlockSizeX(32);
   __hmppcg_call.setBlockSizeY(8LL);
   __hmppcg_call.addLocalParameter(&c1c5, 8, "c1c5_1");
   __hmppcg_call.addLocalParameter(&c2dttx1, 8, "c2dttx1_1");
   __hmppcg_call.addLocalParameter(&c3c4, 8, "c3c4_1");
   __hmppcg_call.addLocalParameter(&comz1, 8, "comz1_1");
   __hmppcg_call.addLocalParameter(&comz4, 8, "comz4_1");
   __hmppcg_call.addLocalParameter(&comz5, 8, "comz5_1");
   __hmppcg_call.addLocalParameter(&comz6, 8, "comz6_1");
   __hmppcg_call.addLocalParameter(&con43, 8, "con43_1");
   __hmppcg_call.addLocalParameter(&dttx1, 8, "dttx1_1");
   __hmppcg_call.addLocalParameter(&dttx2, 8, "dttx2_1");
   __hmppcg_call.addLocalParameter(&dx1, 8, "dx1_1");
   __hmppcg_call.addLocalParameter(&dx2, 8, "dx2_1");
   __hmppcg_call.addLocalParameter(&dx5, 8, "dx5_1");
   __hmppcg_call.addLocalParameter(&dxmax, 8, "dxmax_1");
   __hmppcg_call.addLocalParameter((hmpprt::s32) (gp0_1), "gp0");
   __hmppcg_call.addLocalParameter(&i_1, 8, "i_9");
   __hmppcg_call.addLocalParameter(&j, 8, "j_13");
   __hmppcg_call.addLocalParameter(&lhsX, 8, "lhsX_1");
   __hmppcg_call.addLocalParameter(&lhsmX, 8, "lhsmX_1");
   __hmppcg_call.addLocalParameter(&lhspX_1, 8, "lhspX");
   __hmppcg_call.addLocalParameter((hmpprt::s32) (ni), "ni_1");
   __hmppcg_call.addLocalParameter((hmpprt::s32) (nx2), "nx2_1");
   __hmppcg_call.addLocalParameter((hmpprt::s32) (ny2), "ny2_1");
   __hmppcg_call.addLocalParameter((hmpprt::s32) (nz2), "nz2_1");
   __hmppcg_call.addLocalParameter(&rho_i, 8, "rho_i_1");
   __hmppcg_call.addLocalParameter(&rhonX, 8, "rhonX_1");
   __hmppcg_call.addLocalParameter(&__hmpp_vla_sizes__rhsX_1, 8, "__hmpp_vla_sizes__rhsX");
   __hmppcg_call.addLocalParameter(&rhsX, 8, "rhsX_1");
   __hmppcg_call.addLocalParameter(&speed, 8, "speed_1");
   __hmppcg_call.addLocalParameter(&us, 8, "us_1");
   __hmppcg_call.launch(__hmpp_acc_region__x_solve_75__bbz1aha1_parallel_region_1, hmpprt::Context::getInstance()->getCUDADevice());
  }
  ;
  # 312 "<preprocessor>"
  k = nz2 + 1;
 }
 else
 {
  # 312 "<preprocessor>"
  k = 1;
 }
 # 312 "<preprocessor>"
 *__hmpp_addr__i = *i_1;
 # 313 "<preprocessor>"
 *__hmpp_addr__j = *j;
 # 314 "<preprocessor>"
 *__hmpp_addr__k = k;
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&lhsX));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&j));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&i_1));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&speed));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&rho_i));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&us));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&rhonX));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&lhsmX));
 # 5 "<preprocessor>"
 hmpprt::Context::getInstance()->free((void **) (&lhspX_1));
}
#endif // __CUDACC__



# 5 "<preprocessor>"

#ifndef __CUDACC__
extern "C" CDLT_API  void __hmpp_acc_region__x_solve_75__bbz1aha1(hmpprt::u64* __hmpp_vla_sizes__rhsX_2, hmpprt::s32 nx2_2, hmpprt::s32 nz2_2, hmpprt::s32* __hmpp_addr__i_1, hmpprt::s32* __hmpp_addr__j_1, hmpprt::s32* __hmpp_addr__k_1, hmpprt::s32 ni_2, hmpprt::s32 gp0_2, double* rhsX_2)
{
 # 1 "<preprocessor>"
 (__hmpp_acc_region__x_solve_75__bbz1aha1_internal_1(hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,hmpprt::u64> (__hmpp_vla_sizes__rhsX_2), nx2_2, nz2_2, __hmpp_addr__i_1, __hmpp_addr__j_1, __hmpp_addr__k_1, ni_2, gp0_2, hmpprt::DevicePtr<hmpprt::MS_CUDA_GLOB,double> (rhsX_2)));
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
      __hmpp_acc_region__x_solve_75__bbz1aha1_parallel_region_1 = new hmpprt::CUDAGrid(hmpprt_module, "__hmpp_acc_region__x_solve_75__bbz1aha1_parallel_region_1");

    }
    hmpprt::Context::getInstance()->getGrouplet()->setTarget(hmpprt::CUDA);
    hmpprt::Context::getInstance()->getGrouplet()->addSignature("__hmpp_acc_region__x_solve_75__bbz1aha1", "prototype __hmpp_acc_region__x_solve_75__bbz1aha1(__hmpp_vla_sizes__rhsX: ^cudaglob u64, nx2: s32, nz2: s32, __hmpp_addr__i: ^host s32, __hmpp_addr__j: ^host s32, __hmpp_addr__k: ^host s32, ni: s32, gp0: s32, rhsX: ^cudaglob double)");

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
      delete __hmpp_acc_region__x_solve_75__bbz1aha1_parallel_region_1;

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
