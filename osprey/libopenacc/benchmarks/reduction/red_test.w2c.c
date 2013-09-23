/*******************************************************
 * C file translated from WHIRL Tue Aug 27 14:34:38 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include "red_test.w2c.h"


__inline unsigned int gnu_dev_major(
  unsigned long long __dev)
{
  
  
  return((unsigned int)(_U8LSHR(__dev, (unsigned long long)(8))) & 4095U) | ((unsigned int)(_U8LSHR(__dev, (unsigned long long)(32))) & 4294963200U);
} /* gnu_dev_major */


__inline unsigned int gnu_dev_minor(
  unsigned long long __dev)
{
  
  
  return((unsigned int)(__dev) & 255U) | ((unsigned int)(_U8LSHR(__dev, (unsigned long long)(12))) & 4294967040U);
} /* gnu_dev_minor */


__inline unsigned long long gnu_dev_makedev(
  unsigned int __major,
  unsigned int __minor)
{
  
  
  return((unsigned long long)(((__minor & 255U) | _U4SHL(__major & 4095U, (unsigned int)(8)))) | _U8SHL((unsigned long long) __minor & 4294967040ULL, (unsigned long long)(12))) | _U8SHL((unsigned long long) __major & 4294963200ULL, (unsigned long long)(32));
} /* gnu_dev_makedev */


extern void test1()
{
  
  int i;
  int j;
  int sum;
  int known_sum;
  int NI;
  int NJ;
  int * input;
  int * _temp__casttmp0;
  int * _temp_call0;
  int __acch_temp__is_pcreate;
  int * __device_input;
  int * __device_reduction_sum;
  
  /*Begin_of_nested_PU(s)*/
  
  NI = 2048;
  NJ = 1024;
  _temp_call0 = malloc((unsigned long long)((long long)(NI * NJ)) * 4ULL);
  _temp__casttmp0 = _temp_call0;
  input = _temp__casttmp0;
  j = 0;
  while(j < NJ)
  {
    _514 :;
    i = 0;
    while(i < NI)
    {
      _1026 :;
      * (input + (long long)(i + (j * NI))) = (i + j) % 10;
      i = i + 1;
      _770 :;
    }
    goto _1282;
    _1282 :;
    j = j + 1;
    _258 :;
  }
  goto _1538;
  _1538 :;
  sum = 0;
  __acch_temp__is_pcreate = __accr_present_create(input, 0U, (unsigned int)(NI * NJ), (unsigned int)(NI * NJ) * 4U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(input, &__device_input, (unsigned int)(NI * NJ) * 4U);
    __accr_memin_h2d(input, __device_input, (unsigned int)(NI * NJ) * 4U, 0U, -2);
  }
  __accr_set_default_gang_vector();
  __accr_reduction_buff_malloc(&__device_reduction_sum, 3);
  __accr_push_kernel_param_scalar(&NJ);
  __accr_push_kernel_param_scalar(&NI);
  __accr_push_kernel_param_pointer(&__device_input);
  __accr_push_kernel_param_scalar(&sum);
  __accr_push_kernel_param_pointer(&__device_reduction_sum, 3);
  __accr_launchkernel("__accrg_test1_1_2\0", "red_test.w2c.ptx\0", -2);
  __accr_final_reduction_algorithm(&sum, __device_reduction_sum, "__accrg_reduction_ADD_int32_1_1\0", 3U);
  __accr_free_on_device(__device_input);
  _7170 :;
  known_sum = 0;
  j = 0;
  while(j < NJ)
  {
    _2818 :;
    i = 0;
    while(i < NI)
    {
      _3330 :;
      known_sum = *(input + (long long)(i + (j * NI))) + known_sum;
      i = i + 1;
      _3074 :;
    }
    goto _3586;
    _3586 :;
    j = j + 1;
    _2562 :;
  }
  goto _3842;
  _3842 :;
  free(input);
  if(sum == known_sum)
  {
    printf("Success!\n\0");
  }
  else
  {
    printf("Failed! sum=%d, known_sum=%d\n\0", sum, known_sum);
  }
  return;
} /* test1 */


extern int main()
{
  
  
  acc_init(1U);
  test1();
  acc_shutdown(1U);
  return 0;
} /* main */

