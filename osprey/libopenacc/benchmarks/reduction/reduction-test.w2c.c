/*******************************************************
 * C file translated from WHIRL Mon Jun 10 15:20:29 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openacc_rtl.h>

int main()
{
  
  double * data;
  int i;
  int N;
  double sum;
  double * _temp__casttmp0;
  double * _temp_call5;
  int __acch_temp__is_pcreate;
  double * __device_data;
  double * __device_reduction_sum;
  
  /*Begin_of_nested_PU(s)*/
  
  N = 2048;
  _temp_call5 = malloc((unsigned int) N * 8U);
  _temp__casttmp0 = _temp_call5;
  data = _temp__casttmp0;
  i = 0;
  while(i < N)
  {
    _514 :;
    * (data + (unsigned int) i) = (double)(i);
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  sum = 0.0;
  __accr_setup();
  __acch_temp__is_pcreate = __accr_present_create(data, 0, N, N * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(data, (void**)&__device_data, N * 8);
    __accr_memin_h2d(data, __device_data, N * 8, 0);
  }
  __accr_set_default_gang_vector();
  __accr_reduction_buff_malloc((void**)&__device_reduction_sum, 10);
//  __accr_set_default_gang_vector();
  __accr_push_kernel_param_pointer((void**)&__device_data);
  //__accr_push_kernel_param_pointer(&N);
 // __accr_push_kernel_param_pointer(__device_reduction_sum, 10);
  __accr_push_kernel_param_scalar(&N);
  __accr_push_kernel_param_pointer((void**)&__device_reduction_sum);
  __accr_launchkernel("__accrg_main_1_1", "reduction-test.w2c.ptx");
  __accr_final_reduction_algorithm(&sum, __device_reduction_sum, 10);
  _1794 :;
  __accr_free_on_device(__device_data);
  _1538 :;
  __accr_cleanup();
  printf("Reduction sum is: %f\n\0", sum);
  return 1;
} /* main */

