/*******************************************************
 * C file translated from WHIRL Wed Jun 12 13:41:30 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <openacc_rtl.h>



int main()
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
  N = 1000000LL;
  __accr_setup();
  gettimeofday(&tim, (void *) 0ULL);
  start = ((double)((tim).tv_usec) / 1.0e+03) + ((double)((tim).tv_sec) * 1.0e+03);
  __accr_set_default_gang_vector();
  //__accr_reduction_buff_malloc(&__device_reduction_pi);
  __accr_reduction_buff_malloc(&__device_reduction_pi, 10);
  //__accr_set_default_gang_vector();
  //__accr_push_kernel_param_pointer(&N);
  __accr_push_kernel_param_scalar(&N);
  __accr_push_kernel_param_pointer(&__device_reduction_pi);
  //__accr_push_kernel_param_pointer(__device_reduction_pi, 10);
  __accr_launchkernel("__accrg_main_1_1", "acc-pi.w2c.ptx");
  //__accr_final_reduction_algorithm(&pi, __device_reduction_pi);
  __accr_final_reduction_algorithm(&pi, __device_reduction_pi, 10);
  _258 :;
  gettimeofday(&tim, (void *) 0ULL);
  end = ((double)((tim).tv_usec) / 1.0e+03) + ((double)((tim).tv_sec) * 1.0e+03);
  __accr_cleanup();
  printf("pi=%16.15f\n\0", pi / (double)(N));
  printf("Time %.2f (ms)\n\0", end - start);
  return 0;
} /* main */

