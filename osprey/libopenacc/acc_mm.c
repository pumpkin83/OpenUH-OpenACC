/*******************************************************
 * C file translated from WHIRL Sun May 19 21:26:28 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc_rtl.h>

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
      * (A + (unsigned int)((j + (n * i)))) = 0.0;
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
      * (A + (unsigned int)((j + (n * i)))) = (double)((((i * j) * 3) / n) / n);
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
      * (B + (unsigned int)((j + (n * i)))) = (double)((((i * j) * 5) / n) / n);
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
      * (C + (unsigned int)((j + (n * i)))) = 0.0;
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
  
  __acch_temp__is_pcreate = __accr_present_create(A, 0, n * n, (n * n) * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(A, &__device_A, (n * n) * 8);
    __accr_memin_h2d(A, __device_A, (n * n) * 8, 0);
  }
  __acch_temp__is_pcreate = __accr_present_create(B, 0, n * n, (n * n) * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(B, &__device_B, (n * n) * 8);
    __accr_memin_h2d(B, __device_B, (n * n) * 8, 0);
  }
  __acch_temp__is_pcreate = __accr_present_create(C, 0, n * n, (n * n) * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(C, &__device_C, (n * n) * 8);
  }
  __accr_push_kernel_param_pointer(&__device_A);
  __accr_push_kernel_param_pointer(&__device_B);
  __accr_push_kernel_param_pointer(&__device_C);
  __accr_push_kernel_param_scalar(&n);
  __accr_set_default_gang_vector();
  __accr_launchkernel("__accrg_iter_matmul_1_1", "acc_mm.ptx");
  __accr_memout_d2h(__device_C, C, (n * n) * 8, 0);
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
      sum = *(C + (unsigned int)((j + (n * i)))) + sum;
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
  register unsigned int _w2c___comma0;
  register unsigned int _w2c___comma1;
  register unsigned int _w2c___comma2;
  int n;
  double * A;
  double * B;
  double * C;
  double start;
  double end;
  struct timeval tim;
  
  if(argc != 2)
  {
    fprintf(stderr, "Usage: matmul <n>\n\0");
    exit(1);
  }
  _w2c___comma = atoi(*(argv + 1LL));
  n = _w2c___comma;
  A = (double*) malloc((unsigned int)((n * n)) * 8U);
  B = (double*) malloc((unsigned int)((n * n)) * 8U);
  C = (double*) malloc((unsigned int)((n * n)) * 8U);
  initA(A, n);
  initB(B, n);
  initC(C, n);
  verify(A, n);
  verify(B, n);
  gettimeofday(&tim, (struct timezone *) 0U);
  start = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  iter_matmul(A, B, C, n);
  gettimeofday(&tim, (struct timezone *) 0U);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  printf("Execution time is: %.2f s\n\0", end - start);
  verify(C, n);
  free(C);
  free(B);
  free(A);
  return 0;
} /* main */

