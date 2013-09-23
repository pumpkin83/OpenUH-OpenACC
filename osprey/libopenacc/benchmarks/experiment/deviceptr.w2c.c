/*******************************************************
 * C file translated from WHIRL Sun Aug 11 13:10:28 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include "deviceptr.w2c.h"


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


extern int main()
{
  
  int i;
  int N;
  int sum;
  int known_sum;
  int * A;
  int * B;
  int * _temp__casttmp0;
  int * _temp__casttmp1;
  int * _temp_call0;
  int * _temp_call1;
  int __acch_temp__is_pcreate;
  int * __device_B;
  
  /*Begin_of_nested_PU(s)*/
  
  N = 4096;
  _temp_call0 = acc_malloc((unsigned int)((unsigned long long)((unsigned long long) N) * 4ULL));
  _temp__casttmp0 = _temp_call0;
  A = _temp__casttmp0;
  _temp_call1 = malloc((unsigned long long)((unsigned long long) N) * 8ULL);
  _temp__casttmp1 = _temp_call1;
  B = _temp__casttmp1;
  acc_init(3U);
  __acch_temp__is_pcreate = __accr_present_create(B, 0U, (unsigned int) N, (unsigned int)(N) * 4U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(B, &__device_B, (unsigned int)(N) * 4U);
  }
  __accr_set_default_gang_vector();
  __accr_set_gang_num_x(8);
  __accr_set_vector_num_x(N / 8);
  __accr_push_kernel_param_scalar(&N);
  __accr_push_kernel_param_pointer(&A);
  __accr_push_kernel_param_pointer(&__device_B);
  __accr_launchkernel("__accrg_main_1_1", "deviceptr.w2c.ptx", -2);
  _2306 :;
  __accr_memout_d2h(__device_B, B, (unsigned int)(N) * 4U, 0U, -2);
  __accr_free_on_device(__device_B);
  _2050 :;
  sum = 0;
  i = 0;
  while(i < N)
  {
    _514 :;
    sum = *(B + (unsigned long long)((unsigned long long) i)) + sum;
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  acc_free(A);
  known_sum = N + ((N * (N + -1)) / 2);
  if(sum == known_sum)
  {
    printf("TEST PASSED!\n\0");
  }
  else
  {
    printf("TEST FAILED!\n\0");
  }
  return 0;
} /* main */

