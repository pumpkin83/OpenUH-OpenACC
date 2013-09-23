/*******************************************************
 * C file translated from WHIRL Mon Aug 19 22:56:48 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include "mm_2.w2c.h"


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


extern void zero(
  double * A,
  int n)
{
  
  int i;
  int j;
  
  i = 0;
  while(n > i)
  {
    _514 :;
    j = 0;
    while(n > j)
    {
      _1026 :;
      * (A + (long long)(j + (n * i))) = 0.0;
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
  return;
} /* zero */


extern void initA(
  double * A,
  int n)
{
  
  int i;
  int j;
  
  i = 0;
  while(n > i)
  {
    _514 :;
    j = 0;
    while(n > j)
    {
      _1026 :;
      * (A + (long long)(j + (n * i))) = (double)((((i * j) * 3) / n) / n);
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
  return;
} /* initA */


extern void initB(
  double * B,
  int n)
{
  
  int i;
  int j;
  
  i = 0;
  while(n > i)
  {
    _514 :;
    j = 0;
    while(n > j)
    {
      _1026 :;
      * (B + (long long)(j + (n * i))) = (double)((((i * j) * 5) / n) / n);
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
  return;
} /* initB */


extern void initC(
  double * C,
  int n)
{
  
  int i;
  int j;
  
  i = 0;
  while(n > i)
  {
    _514 :;
    j = 0;
    while(n > j)
    {
      _1026 :;
      * (C + (long long)(j + (n * i))) = 0.0;
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
  return;
} /* initC */


extern void iter_matmul(
  double * A,
  double * B,
  double * C,
  int n)
{
  
  int __acch_temp__is_pcreate;
  double * __device_A;
  double * __device_B;
  double * __device_C;
  
  /*Begin_of_nested_PU(s)*/
  
  __acch_temp__is_pcreate = __accr_present_create(A, 0U, (unsigned int)(n * n), (unsigned int)(n * n) * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(A, &__device_A, (unsigned int)(n * n) * 8U);
    __accr_memin_h2d(A, __device_A, (unsigned int)(n * n) * 8U, 0U, -2);
  }
  __acch_temp__is_pcreate = __accr_present_create(B, 0U, (unsigned int)(n * n), (unsigned int)(n * n) * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(B, &__device_B, (unsigned int)(n * n) * 8U);
    __accr_memin_h2d(B, __device_B, (unsigned int)(n * n) * 8U, 0U, -2);
  }
  __acch_temp__is_pcreate = __accr_present_create(C, 0U, (unsigned int)(n * n), (unsigned int)(n * n) * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(C, &__device_C, (unsigned int)(n * n) * 8U);
  }
  __accr_set_default_gang_vector();
  __accr_set_gang_num_y(512);
  __accr_set_gang_num_x(8);
  __accr_set_vector_num_y(2);
  __accr_push_kernel_param_scalar(&n);
  __accr_push_kernel_param_pointer(&__device_A);
  __accr_push_kernel_param_pointer(&__device_B);
  __accr_push_kernel_param_pointer(&__device_C);
  __accr_launchkernel("__accrg_iter_matmul_1_1\0", "mm_2.w2c.ptx\0", -2);
  _1794 :;
  __accr_memout_d2h(__device_C, C, (unsigned int)(n * n) * 8U, 0U, -2);
  __accr_free_on_device(__device_A);
  __accr_free_on_device(__device_B);
  __accr_free_on_device(__device_C);
  _1538 :;
  return;
} /* iter_matmul */


extern void verify(
  double * C,
  int n)
{
  
  int i;
  int j;
  double sum;
  
  sum = 0.0;
  i = 0;
  while(n > i)
  {
    _514 :;
    j = 0;
    while(n > j)
    {
      _1026 :;
      sum = *(C + (long long)(j + (n * i))) + sum;
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
  printf("Sum of C is: %f\n\0", sum);
  return;
} /* verify */


extern int main(
  int argc,
  char ** argv)
{
  
  register int _w2c___comma;
  int n;
  double * A;
  double * B;
  double * C;
  double start;
  double end;
  struct timeval tim;
  double * _temp__casttmp0;
  double * _temp__casttmp1;
  double * _temp__casttmp2;
  double * _temp_call15;
  double * _temp_call16;
  double * _temp_call17;
  
  if(argc != 2)
  {
    fprintf(stderr, "Usage: matmul <n>\n\0");
    exit(1);
  }
  _w2c___comma = atoi(*(argv + 1LL));
  n = _w2c___comma;
  _temp_call15 = malloc((unsigned long long)((long long)(n * n)) * 8ULL);
  _temp__casttmp0 = _temp_call15;
  A = _temp__casttmp0;
  _temp_call16 = malloc((unsigned long long)((long long)(n * n)) * 8ULL);
  _temp__casttmp1 = _temp_call16;
  B = _temp__casttmp1;
  _temp_call17 = malloc((unsigned long long)((long long)(n * n)) * 8ULL);
  _temp__casttmp2 = _temp_call17;
  C = _temp__casttmp2;
  initA(A, n);
  initB(B, n);
  initC(C, n);
  acc_init(1U);
  gettimeofday(&tim, (struct timezone *) 0ULL);
  start = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  iter_matmul(A, B, C, n);
  gettimeofday(&tim, (struct timezone *) 0ULL);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  printf("Execution time is: %.2f s\n\0", end - start);
  verify(C, n);
  free(C);
  free(B);
  free(A);
  return 0;
} /* main */

