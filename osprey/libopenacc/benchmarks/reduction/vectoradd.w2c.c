/*******************************************************
 * C file translated from WHIRL Mon Jun 10 17:32:15 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(
  int argc,
  char ** argv)
{
  
  int n;
  unsigned int bytes;
  double * a;
  double * b;
  double * c;
  int i;
  double diff;
  double * _temp__casttmp0;
  double * _temp__casttmp1;
  double * _temp__casttmp2;
  double sum;
  double * _temp_call5;
  double * _temp_call6;
  double * _temp_call7;
  int __acch_temp__is_pcreate;
  double * __device_a;
  double * __device_b;
  double * __device_c;
  double * __device_reduction_diff;
  
  /*Begin_of_nested_PU(s)*/
  
  n = 4096;
  bytes = (unsigned int) n * 8U;
  _temp_call5 = malloc(bytes);
  _temp__casttmp0 = _temp_call5;
  a = _temp__casttmp0;
  _temp_call6 = malloc(bytes);
  _temp__casttmp1 = _temp_call6;
  b = _temp__casttmp1;
  _temp_call7 = malloc(bytes);
  _temp__casttmp2 = _temp_call7;
  c = _temp__casttmp2;
  i = 0;
  while(n > i)
  {
    _514 :;
    * (a + (unsigned int) i) = sin((double)(i)) * sin((double)(i));
    * (b + (unsigned int) i) = cos((double)(i)) * cos((double)(i));
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;

	__accr_setup();
  __acch_temp__is_pcreate = __accr_present_create(a, 0, n, n * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(a, &__device_a, n * 8);
    __accr_memin_h2d(a, __device_a, n * 8, 0);
  }
  __acch_temp__is_pcreate = __accr_present_create(b, 0, n, n * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(b, &__device_b, n * 8);
    __accr_memin_h2d(b, __device_b, n * 8, 0);
  }
  __acch_temp__is_pcreate = __accr_present_create(c, 0, n, n * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(c, &__device_c, n * 8);
  }
  __accr_set_default_gang_vector();
  __accr_reduction_buff_malloc(&__device_reduction_diff, 10);
//  __accr_set_default_gang_vector();
  __accr_push_kernel_param_pointer(&__device_a);
  __accr_push_kernel_param_pointer(&__device_b);
  __accr_push_kernel_param_pointer(&__device_c);
  //__accr_push_kernel_param_pointer(&n);
  //__accr_push_kernel_param_pointer(__device_reduction_diff, 10);
  __accr_push_kernel_param_scalar(&n);
  __accr_push_kernel_param_pointer(&__device_reduction_diff);
  __accr_launchkernel("__accrg_main_1_1", "vectoradd.w2c.ptx");
  __accr_final_reduction_algorithm(&diff, __device_reduction_diff);
  _3074 :;
  __accr_memout_d2h(__device_c, c, n * 8, 0);
  __accr_free_on_device(__device_a);
  __accr_free_on_device(__device_b);
  __accr_free_on_device(__device_c);

  __accr_cleanup();
  _2818 :;
  sum = 0.0;
  i = 0;
  while(n > i)
  {
    _1282 :;
    sum = *(c + (unsigned int) i) + sum;
    i = i + 1;
    _1026 :;
  }
  goto _1538;
  _1538 :;
  sum = sum / (double)(n);
  printf("GPU Reduction result: %f\n", diff/n);
  printf("CPU Reduction result: %f\n\0", sum);
  return 0;
} /* main */

