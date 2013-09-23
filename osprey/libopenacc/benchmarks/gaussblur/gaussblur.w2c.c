/*******************************************************
 * C file translated from WHIRL Sun Aug  4 10:39:02 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <openacc_rtl.h>




extern int gaussblur(
  int nx,
  int ny,
  const double s0,
  const double s1,
  const double s2,
  const double s4,
  const double s5,
  const double s8,
  double * w0,
  double * w1)
{
  
  double f;
  unsigned int szarray;
  double * __device_w0;
  double * __device_w1;
  
  /*Begin_of_nested_PU(s)*/
  
  f = 1.0 / ((s0 + ((s8 + (s4 + (s1 + s2))) * 4.0)) + (s5 * 8.0));
  szarray = (unsigned int) nx * (unsigned int) ny;
  __accr_get_device_addr(w0, &__device_w0, 0U, szarray * 8U);
  __accr_get_device_addr(w1, &__device_w1, 0U, szarray * 8U);
  __accr_set_default_gang_vector();
  __accr_set_gang_num_x(512);
  __accr_set_vector_num_y(2);
  __accr_set_vector_num_x(128);
  __accr_push_kernel_param_scalar(&ny);
  __accr_push_kernel_param_scalar(&nx);
  __accr_push_kernel_param_scalar(&f);
  __accr_push_kernel_param_pointer(&__device_w0);
  __accr_push_kernel_param_scalar(&s0);
  __accr_push_kernel_param_scalar(&s1);
  __accr_push_kernel_param_scalar(&s2);
  __accr_push_kernel_param_scalar(&s4);
  __accr_push_kernel_param_scalar(&s5);
  __accr_push_kernel_param_scalar(&s8);
  __accr_push_kernel_param_pointer(&__device_w1);
  __accr_launchkernel("__accrg_gaussblur_1_1", "gaussblur.w2c.ptx");
  _514 :;
  _258 :;
  return 0;
} /* gaussblur */


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
  register int _w2c___comma7;
  register int _w2c___comma8;
  register int _w2c___comma9;
  int i;
  int it;
  struct timeval tim;
  double start;
  double end;
  int nx;
  int ny;
  int nt;
  double s0;
  double s1;
  double s2;
  double s4;
  double s5;
  double s8;
  unsigned int szarray;
  unsigned int szarrayb;
  double * w0;
  double * _temp__casttmp0;
  double * w1;
  double * _temp__casttmp1;
  double mean;
  double * w;
  double * _temp_call1;
  double * _temp_call2;
  int __acch_temp__is_pcreate;
  double * __device_w0;
  double * __device_w1;
  
  /*Begin_of_nested_PU(s)*/
  
  if(argc != 4)
  {
    printf("Usage: %s <nx> <ny> <nt>\n\0", *argv);
    exit(1);
  }
  srand(17U);
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
  _w2c___comma2 = rand();
  s0 = (double)(_w2c___comma2) / 2.147483647e+09;
  _w2c___comma3 = rand();
  s1 = (double)(_w2c___comma3) / 2.147483647e+09;
  _w2c___comma4 = rand();
  s2 = (double)(_w2c___comma4) / 2.147483647e+09;
  _w2c___comma5 = rand();
  s4 = (double)(_w2c___comma5) / 2.147483647e+09;
  _w2c___comma6 = rand();
  s5 = (double)(_w2c___comma6) / 2.147483647e+09;
  _w2c___comma7 = rand();
  s8 = (double)(_w2c___comma7) / 2.147483647e+09;
  printf("s0 = %f, s1 = %f, s2 = %f\n\0", s0, s1, s2);
  printf("s4 = %f, s5 = %f, s8 = %f\n\0", s4, s5, s8);
  szarray = (unsigned int) nx * (unsigned int) ny;
  szarrayb = (unsigned long long) szarray * 8ULL;
  _temp_call1 = malloc((unsigned long long) szarrayb);
  _temp__casttmp0 = _temp_call1;
  w0 = _temp__casttmp0;
  _temp_call2 = malloc((unsigned long long) szarrayb);
  _temp__casttmp1 = _temp_call2;
  w1 = _temp__casttmp1;
  if(((unsigned long long)(w0) == 0ULL) || ((unsigned long long)(w1) == 0ULL))
  {
    printf("Error allocating memory for arrays: %p, %p\n\0", w0, w1);
    exit(1);
  }
  mean = 0.0;
  i = 0;
  while((unsigned int) i < szarray)
  {
    _514 :;
    _w2c___comma8 = rand();
    * (w0 + (unsigned long long)((unsigned long long) i)) = (double)(_w2c___comma8) / 2.147483647e+09;
    _w2c___comma9 = rand();
    * (w1 + (unsigned long long)((unsigned long long) i)) = (double)(_w2c___comma9) / 2.147483647e+09;
    mean = mean + (*(w0 + (unsigned long long)((unsigned long long) i)) + *(w1 + (unsigned long long)((unsigned long long) i)));
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  printf("initial mean = %f\n\0", (mean / (double)(szarray)) * 5.0e-01);
  acc_init(1U);
  gettimeofday(&tim, (struct timezone *) 0ULL);
  start = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  __acch_temp__is_pcreate = __accr_present_create(w0, 0U, szarray, szarray * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(w0, &__device_w0, szarray * 8U);
  }
  __acch_temp__is_pcreate = __accr_present_create(w1, 0U, szarray, szarray * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(w1, &__device_w1, szarray * 8U);
  }
  __accr_update_device_variable(w0, (int) 0U, (int)(szarray * 8U));
  __accr_update_device_variable(w1, (int) 0U, (int)(szarray * 8U));
  it = 0;
  while(it < nt)
  {
    _1282 :;
    gaussblur(nx, ny, s0, s1, s2, s4, s5, s8, w0, w1);
    w = w0;
    w0 = w1;
    w1 = w;
    it = it + 1;
    _1026 :;
  }
  goto _1538;
  _1538 :;
  __accr_update_host_variable(w0, (int) 0U, (int)(szarray * 8U));
  __accr_free_on_device(__device_w0);
  __accr_free_on_device(__device_w1);
  _6914 :;
  gettimeofday(&tim, (struct timezone *) 0ULL);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  mean = 0.0;
  i = 0;
  while((unsigned int) i < szarray)
  {
    _2050 :;
    mean = *(w0 + (unsigned long long)((unsigned long long) i)) + mean;
    i = i + 1;
    _1794 :;
  }
  goto _2306;
  _2306 :;
  printf("final mean = %f\n\0", mean / (double)(szarray));
  printf("Time for computing: %.2f s\n\0", end - start);
  free(w0);
  free(w1);
  fflush(stdout);
  return 0;
} /* main */

