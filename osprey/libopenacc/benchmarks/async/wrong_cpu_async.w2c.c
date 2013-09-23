/*******************************************************
 * C file translated from WHIRL Tue Aug 13 23:31:14 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include "cpu_async.w2c.h"


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


extern float get_sum(
  float * array,
  int n)
{
  
  int i;
  int j;
  float sum;
  
  sum = 0.0F;
  i = 0;
  while(n > i)
  {
    _514 :;
    j = 0;
    while(n > j)
    {
      _1026 :;
      sum = *(array + (long long)(j + (n * i))) + sum;
      j = j + 1;
      _770 :;
    }
    goto _1282;
    _1282 :;
    i = i + 1;
    _258 :;
  }
  goto _1538;
  _1538 :;
  return sum;
} /* get_sum */


extern int main(
  int argc,
  char ** argv)
{
  
  register int _w2c___comma;
  register int _w2c___comma0;
  register int _w2c___comma1;
  register int _w2c___comma2;
  register float _w2c___comma3;
  register float _w2c___comma4;
  int n1;
  int n2;
  int i;
  int j;
  int k;
  unsigned int dev_bytes;
  unsigned int host_bytes;
  float * a1;
  float * b1;
  float * c1;
  float * a2;
  float * b2;
  float * c2;
  int async_scalar;
  struct timeval tim;
  double start;
  double end;
  float device_sum;
  float host_sum;
  float * _temp__casttmp0;
  float * _temp__casttmp1;
  float * _temp__casttmp2;
  float * _temp__casttmp3;
  float * _temp__casttmp4;
  float * _temp__casttmp5;
  float tmp;
  float * _temp_call0;
  float * _temp_call1;
  float * _temp_call2;
  float * _temp_call3;
  float * _temp_call4;
  float * _temp_call5;
  int __acch_temp__is_pcreate;
  float * __device_a1;
  float * __device_b1;
  float * __device_c1;
  
  /*Begin_of_nested_PU(s)*/
  
  n1 = 8192;
  n2 = 1024;
  async_scalar = 9;
  srand(1234U);
  dev_bytes = (unsigned long long)((long long)(n1 * n1)) * 4ULL;
  _temp_call0 = valloc((unsigned long long) dev_bytes);
  _temp__casttmp0 = _temp_call0;
  a1 = _temp__casttmp0;
  _temp_call1 = valloc((unsigned long long) dev_bytes);
  _temp__casttmp1 = _temp_call1;
  b1 = _temp__casttmp1;
  _temp_call2 = valloc((unsigned long long) dev_bytes);
  _temp__casttmp2 = _temp_call2;
  c1 = _temp__casttmp2;
  host_bytes = (unsigned long long)((long long)(n2 * n2)) * 4ULL;
  _temp_call3 = malloc((unsigned long long) host_bytes);
  _temp__casttmp3 = _temp_call3;
  a2 = _temp__casttmp3;
  _temp_call4 = malloc((unsigned long long) host_bytes);
  _temp__casttmp4 = _temp_call4;
  b2 = _temp__casttmp4;
  _temp_call5 = malloc((unsigned long long) host_bytes);
  _temp__casttmp5 = _temp_call5;
  c2 = _temp__casttmp5;
  i = 0;
  while(n1 > i)
  {
    _514 :;
    j = 0;
    while(n1 > j)
    {
      _1026 :;
      _w2c___comma = rand();
      * (a1 + (long long)(j + (n1 * i))) = (float)(_w2c___comma) * 4.6566128731e-10F;
      _w2c___comma0 = rand();
      * (b1 + (long long)(j + (n1 * i))) = (float)(_w2c___comma0) * 4.6566128731e-10F;
      * (c1 + (long long)(j + (n1 * i))) = 0.0F;
      j = j + 1;
      _770 :;
    }
    goto _1282;
    _1282 :;
    i = i + 1;
    _258 :;
  }
  goto _1538;
  _1538 :;
  i = 0;
  while(n2 > i)
  {
    _2050 :;
    j = 0;
    while(n2 > j)
    {
      _2562 :;
      _w2c___comma1 = rand();
      * (a2 + (long long)(j + (n2 * i))) = (float)(_w2c___comma1) * 4.6566128731e-10F;
      _w2c___comma2 = rand();
      * (b2 + (long long)(j + (n2 * i))) = (float)(_w2c___comma2) * 4.6566128731e-10F;
      * (c2 + (long long)(j + (n2 * i))) = 0.0F;
      j = j + 1;
      _2306 :;
    }
    goto _2818;
    _2818 :;
    i = i + 1;
    _1794 :;
  }
  goto _3074;
  _3074 :;
  acc_init(3U);
  gettimeofday(&tim, (struct timezone *) 0ULL);
  start = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  __acch_temp__is_pcreate = __accr_present_create(a1, 0U, (unsigned int)(n1 * n1), (unsigned int)(n1 * n1) * 4U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(a1, &__device_a1, (unsigned int)(n1 * n1) * 4U);
  }
  __acch_temp__is_pcreate = __accr_present_create(b1, 0U, (unsigned int)(n1 * n1), (unsigned int)(n1 * n1) * 4U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(b1, &__device_b1, (unsigned int)(n1 * n1) * 4U);
  }
  __acch_temp__is_pcreate = __accr_present_create(c1, 0U, (unsigned int)(n1 * n1), (unsigned int)(n1 * n1) * 4U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(c1, &__device_c1, (unsigned int)(n1 * n1) * 4U);
  }
  __accr_update_device_variable(a1, (int) 0U, (int)((unsigned int)(n1 * n1) * 4U), async_scalar);
  __accr_update_device_variable(b1, (int) 0U, (int)((unsigned int)(n1 * n1) * 4U), async_scalar);
  __accr_set_default_gang_vector();
  __accr_set_gang_num_y(256);
  __accr_set_gang_num_x(256);
  __accr_set_vector_num_y(32);
  __accr_set_vector_num_x(32);
  __accr_push_kernel_param_scalar(&n1);
  __accr_push_kernel_param_pointer(&__device_a1);
  __accr_push_kernel_param_pointer(&__device_b1);
  __accr_push_kernel_param_pointer(&__device_c1);
  __accr_launchkernel("__accrg_main_1_1", "cpu_async.w2c.ptx", async_scalar);
  _10754 :;
  __accr_update_host_variable(c1, (int) 0U, (int)((unsigned int)(n1 * n1) * 4U), async_scalar);
  i = 0;
  while(n2 > i)
  {
    _4354 :;
    j = 0;
    while(n2 > j)
    {
      _4866 :;
      tmp = 0.0F;
      k = 0;
      while(n2 > k)
      {
        _5378 :;
        tmp = tmp + (*(a2 + (long long)(k + (n2 * i))) ** (b2 + (long long)(j + (n2 * k))));
        k = k + 1;
        _5122 :;
      }
      goto _5634;
      _5634 :;
      * (c2 + (long long)(j + (n2 * i))) = tmp;
      j = j + 1;
      _4610 :;
    }
    goto _5890;
    _5890 :;
    i = i + 1;
    _4098 :;
  }
  goto _6146;
  _6146 :;
  __accr_wait_some_or_all_stream(async_scalar);
  __accr_free_on_device(__device_a1);
  __accr_free_on_device(__device_b1);
  __accr_free_on_device(__device_c1);
  _10498 :;
  gettimeofday(&tim, (struct timezone *) 0ULL);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  printf("Time for runtime async computing: %.2f s\n\0", end - start);
  _w2c___comma3 = get_sum(c1, n1);
  device_sum = _w2c___comma3;
  _w2c___comma4 = get_sum(c2, n2);
  host_sum = _w2c___comma4;
  printf("Device sum: %f\n\0", (double)(device_sum));
  printf("Host sum: %f\n\0", (double)(host_sum));
  acc_shutdown(3U);
  return 0;
} /* main */

