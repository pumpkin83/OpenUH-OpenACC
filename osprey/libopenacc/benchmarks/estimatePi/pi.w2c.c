/*******************************************************
 * C file translated from WHIRL Tue Jun 18 00:02:54 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <openacc_rtl.h>




extern int main()
{
  
  double pi;
  long long N;
  struct timeval tim;
  double start;
  double end;
  double * __device_reduction_pi;
  
  /*Begin_of_nested_PU(s)*/
  
  pi = 0.0;
  printf("Estimate Pi OpenACC\n\0");
  N = 10000000LL;
  acc_init(1U);
  gettimeofday(&tim, (struct timezone *) 0ULL);
  start = ((double)((tim).tv_usec) / 1.0e+03) + ((double)((tim).tv_sec) * 1.0e+03);
  __accr_set_default_gang_vector();
  __accr_set_gang_num_x(7813);
  __accr_set_vector_num_x(128);
  __accr_reduction_buff_malloc(&__device_reduction_pi, 10);
  __accr_push_kernel_param_scalar(&N);
  __accr_push_kernel_param_pointer(&__device_reduction_pi);
  __accr_launchkernel("__accrg_main_1_1", "pi.w2c.ptx");
  __accr_final_reduction_algorithm(&pi, __device_reduction_pi, 10);
  _258 :;
  gettimeofday(&tim, (struct timezone *) 0ULL);
  end = ((double)((tim).tv_usec) / 1.0e+03) + ((double)((tim).tv_sec) * 1.0e+03);
  printf("pi=%16.15f\n\0", pi / (double)(N));
  printf("Time %.2f (ms)\n\0", end - start);
  return 0;
} /* main */

