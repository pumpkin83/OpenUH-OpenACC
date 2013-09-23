/*******************************************************
 * C file translated from WHIRL Wed Aug 28 10:22:59 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include "jacobi.w2c.h"


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


extern void jacobi_(
  int nx,
  int ny,
  double c0,
  double c1,
  double c2,
  double * w0,
  double * w1)
{
  
  double * __device_w0;
  double * __device_w1;
  
  /*Begin_of_nested_PU(s)*/
  
  __accr_get_device_addr(w0, &__device_w0, 0U, (unsigned int)(nx * ny) * 8U);
  __accr_get_device_addr(w1, &__device_w1, 0U, (unsigned int)(nx * ny) * 8U);
  __accr_set_default_gang_vector();
  __accr_push_kernel_param_scalar(&ny);
  __accr_push_kernel_param_scalar(&nx);
  __accr_push_kernel_param_pointer(&__device_w0);
  __accr_push_kernel_param_scalar(&c0);
  __accr_push_kernel_param_scalar(&c1);
  __accr_push_kernel_param_scalar(&c2);
  __accr_push_kernel_param_pointer(&__device_w1);
  __accr_launchkernel("__accrg_jacobi__1_1\0", "jacobi.w2c.ptx\0", -2);
  _514 :;
  _258 :;
  return;
} /* jacobi_ */


extern int main(
  int argc,
  char ** argv)
{
  
  register int _w2c___comma;
  register int _w2c___comma0;
  register int _w2c___comma1;
  register int _w2c___comma2;
  register int _w2c___comma3;
  register int _w2c___comma4;
  register int _w2c___comma5;
  register int _w2c___comma6;
  int i;
  int it;
  double mean;
  struct timeval tim;
  double start;
  double end;
  int nx;
  int ny;
  int nt;
  double c0;
  double c1;
  double c2;
  int szarray;
  unsigned int szarrayb;
  double * w0;
  double * _temp__casttmp0;
  double * w1;
  double * _temp__casttmp1;
  double * w;
  double * _temp_call3;
  double * _temp_call4;
  int __acch_temp__is_pcreate;
  double * __device_w0;
  double * __device_w1;
  
  /*Begin_of_nested_PU(s)*/
  
  if(argc != 4)
  {
    printf("Usage: %s <nx> <ny> <nt>\n\0", *argv);
    exit(1);
  }
  _w2c___comma = atoi(*(argv + 1LL));
  nx = _w2c___comma;
  if(nx < 0)
  {
    printf("Value for nx is invalid: %d\n\0", nx);
    exit(1);
  }
  _w2c___comma0 = atoi(*(argv + 2LL));
  ny = _w2c___comma0;
  if(ny < 0)
  {
    printf("Value for ny is invalid: %d\n\0", ny);
    exit(1);
  }
  _w2c___comma1 = atoi(*(argv + 3LL));
  nt = _w2c___comma1;
  if(nt < 0)
  {
    printf("Value for nt is invalid: %d\n\0", nt);
    exit(1);
  }
  srand(17U);
  _w2c___comma2 = rand();
  c0 = (double)(_w2c___comma2) / 2.147483647e+09;
  _w2c___comma3 = rand();
  c1 = ((double)(_w2c___comma3) / 2.147483647e+09) * 2.5e-01;
  _w2c___comma4 = rand();
  c2 = ((double)(_w2c___comma4) / 2.147483647e+09) * 2.5e-01;
  printf("c0 = %f, c1 = %f, c2 = %f\n\0", c0, c1, c2);
  szarray = nx * ny;
  szarrayb = (unsigned long long)((unsigned long long) szarray) * 8ULL;
  _temp_call3 = malloc((unsigned long long) szarrayb);
  _temp__casttmp0 = _temp_call3;
  w0 = _temp__casttmp0;
  _temp_call4 = malloc((unsigned long long) szarrayb);
  _temp__casttmp1 = _temp_call4;
  w1 = _temp__casttmp1;
  if(((unsigned long long)(w0) == 0ULL) || ((unsigned long long)(w1) == 0ULL))
  {
    printf("Error allocating memory for arrays: %p, %p\n\0", w0, w1);
    exit(1);
  }
  i = 0;
  while(i < szarray)
  {
    _514 :;
    _w2c___comma5 = rand();
    * (w0 + (unsigned long long)((unsigned long long) i)) = (double)(_w2c___comma5) / 2.147483647e+09;
    _w2c___comma6 = rand();
    * (w1 + (unsigned long long)((unsigned long long) i)) = (double)(_w2c___comma6) / 2.147483647e+09;
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  acc_init(1U);
  gettimeofday(&tim, (struct timezone *) 0ULL);
  start = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  __acch_temp__is_pcreate = __accr_present_create(w0, 0U, (unsigned int) szarray, (unsigned int)(szarray) * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(w0, &__device_w0, (unsigned int)(szarray) * 8U);
  }
  __acch_temp__is_pcreate = __accr_present_create(w1, 0U, (unsigned int) szarray, (unsigned int)(szarray) * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(w1, &__device_w1, (unsigned int)(szarray) * 8U);
  }
  __accr_update_device_variable(w0, (int) 0U, (int)((unsigned int)(szarray) * 8U), -2);
  __accr_update_device_variable(w1, (int) 0U, (int)((unsigned int)(szarray) * 8U), -2);
  it = 0;
  while(it < nt)
  {
    _1282 :;
    jacobi_(nx, ny, c0, c1, c2, w0, w1);
    w = w0;
    w0 = w1;
    w1 = w;
    it = it + 1;
    _1026 :;
  }
  goto _1538;
  _1538 :;
  __accr_update_host_variable(w0, (int) 0U, (int)((unsigned int)(szarray) * 8U), -2);
  __accr_free_on_device(__device_w0);
  __accr_free_on_device(__device_w1);
  _6914 :;
  gettimeofday(&tim, (struct timezone *) 0ULL);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  mean = 0.0;
  i = 0;
  while(i < szarray)
  {
    _2050 :;
    mean = *(w0 + (unsigned long long)((unsigned long long) i)) + mean;
    i = i + 1;
    _1794 :;
  }
  goto _2306;
  _2306 :;
  printf("Final mean = %f\n\0", mean / (double)(szarray));
  printf("Time for computing: %.2f s\n\0", end - start);
  free(w0);
  free(w1);
  return 0;
} /* main */

