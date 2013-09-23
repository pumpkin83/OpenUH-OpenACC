/*******************************************************
 * C file translated from WHIRL Sun Aug  4 11:01:15 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <openacc_rtl.h>




extern int wave13pt(
  int nx,
  int ny,
  int ns,
  const double c0,
  const double c1,
  const double c2,
  double * w0,
  double * w1,
  double * w2)
{
  
  unsigned int szarray;
  double * __device_w0;
  double * __device_w1;
  double * __device_w2;
  
  /*Begin_of_nested_PU(s)*/
  
  szarray = (unsigned int) ns * ((unsigned int) nx * (unsigned int) ny);
  __accr_get_device_addr(w0, &__device_w0, 0U, szarray * 8U);
  __accr_get_device_addr(w1, &__device_w1, 0U, szarray * 8U);
  __accr_get_device_addr(w2, &__device_w2, 0U, szarray * 8U);
  __accr_set_default_gang_vector();
  __accr_set_gang_num_x(128);
  __accr_set_gang_num_y(31);
  __accr_set_vector_num_y(4);
  __accr_set_vector_num_x(128);
  __accr_push_kernel_param_scalar(&ns);
  __accr_push_kernel_param_scalar(&ny);
  __accr_push_kernel_param_scalar(&nx);
  __accr_push_kernel_param_scalar(&c1);
  __accr_push_kernel_param_pointer(&__device_w1);
  __accr_push_kernel_param_scalar(&c0);
  __accr_push_kernel_param_pointer(&__device_w0);
  __accr_push_kernel_param_scalar(&c2);
  __accr_push_kernel_param_pointer(&__device_w2);
  __accr_launchkernel("__accrg_wave13pt_1_1", "wave13pt.w2c.ptx");
  _514 :;
  _258 :;
  return 0;
} /* wave13pt */


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
  int i;
  int it;
  struct timeval tim;
  double start;
  double end;
  int nx;
  int ny;
  int ns;
  int nt;
  double c0;
  double c1;
  double c2;
  unsigned int szarray;
  unsigned int szarrayb;
  double * w0;
  double * _temp__casttmp0;
  double * w1;
  double * _temp__casttmp1;
  double * w2;
  double * _temp__casttmp2;
  double mean;
  double * w;
  double * _temp_call1;
  double * _temp_call2;
  double * _temp_call3;
  int __acch_temp__is_pcreate;
  double * __device_w0;
  double * __device_w1;
  double * __device_w2;
  
  /*Begin_of_nested_PU(s)*/
  
  if(argc != 5)
  {
    printf("Usage: %s <nx> <ny> <ns> <nt>\n\0", *argv);
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
  ns = _w2c___comma1;
  if(ns < 0)
  {
    printf("Value for ns is invalid: %d\n\0", ns);
    exit(1);
  }
  _w2c___comma2 = atoi(*(argv + 4LL));
  nt = _w2c___comma2;
  if(nt < 0)
  {
    printf("Value for nt is invalid: %d\n\0", nt);
    exit(1);
  }
  _w2c___comma3 = rand();
  c0 = (double)(_w2c___comma3) / 2.147483647e+09;
  _w2c___comma4 = rand();
  c1 = ((double)(_w2c___comma4) / 2.147483647e+09) / 6.0;
  _w2c___comma5 = rand();
  c2 = ((double)(_w2c___comma5) / 2.147483647e+09) / 6.0;
  printf("c0 = %f, c1 = %f, c2 = %f\n\0", c0, c1, c2);
  szarray = (unsigned int) ns * ((unsigned int) nx * (unsigned int) ny);
  szarrayb = (unsigned long long) szarray * 8ULL;
  _temp_call1 = malloc((unsigned long long) szarrayb);
  _temp__casttmp0 = _temp_call1;
  w0 = _temp__casttmp0;
  _temp_call2 = malloc((unsigned long long) szarrayb);
  _temp__casttmp1 = _temp_call2;
  w1 = _temp__casttmp1;
  _temp_call3 = malloc((unsigned long long) szarrayb);
  _temp__casttmp2 = _temp_call3;
  w2 = _temp__casttmp2;
  if((((unsigned long long)(w0) == 0ULL) || ((unsigned long long)(w1) == 0ULL)) || ((unsigned long long)(w2) == 0ULL))
  {
    printf("Error allocating memory for arrays: %p, %p, %p\n\0", w0, w1, w2);
    exit(1);
  }
  mean = 0.0;
  i = 0;
  while((unsigned int) i < szarray)
  {
    _514 :;
    _w2c___comma6 = rand();
    * (w0 + (unsigned long long)((unsigned long long) i)) = (double)(_w2c___comma6) / 2.147483647e+09;
    _w2c___comma7 = rand();
    * (w1 + (unsigned long long)((unsigned long long) i)) = (double)(_w2c___comma7) / 2.147483647e+09;
    _w2c___comma8 = rand();
    * (w2 + (unsigned long long)((unsigned long long) i)) = (double)(_w2c___comma8) / 2.147483647e+09;
    mean = mean + (*(w2 + (unsigned long long)((unsigned long long) i)) + (*(w0 + (unsigned long long)((unsigned long long) i)) + *(w1 + (unsigned long long)((unsigned long long) i))));
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  printf("initial mean = %f\n\0", (mean / (double)(szarray)) / 3.0);
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
  __acch_temp__is_pcreate = __accr_present_create(w2, 0U, szarray, szarray * 8U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(w2, &__device_w2, szarray * 8U);
  }
  __accr_update_device_variable(w0, (int) 0U, (int)(szarray * 8U));
  __accr_update_device_variable(w1, (int) 0U, (int)(szarray * 8U));
  __accr_update_device_variable(w2, (int) 0U, (int)(szarray * 8U));
  it = 0;
  while(it < nt)
  {
    _1282 :;
    wave13pt(nx, ny, ns, c0, c1, c2, w0, w1, w2);
    w = w0;
    w0 = w1;
    w1 = w2;
    w2 = w;
    it = it + 1;
    _1026 :;
  }
  goto _1538;
  _1538 :;
  __accr_update_host_variable(w1, (int) 0U, (int)(szarray * 8U));
  __accr_free_on_device(__device_w0);
  __accr_free_on_device(__device_w1);
  __accr_free_on_device(__device_w2);
  _7682 :;
  gettimeofday(&tim, (struct timezone *) 0ULL);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  mean = 0.0;
  i = 0;
  while((unsigned int) i < szarray)
  {
    _2050 :;
    mean = *(w1 + (unsigned long long)((unsigned long long) i)) + mean;
    i = i + 1;
    _1794 :;
  }
  goto _2306;
  _2306 :;
  printf("final mean = %f\n\0", mean / (double)(szarray));
  printf("Time for computing: %.2f s\n\0", end - start);
  free(w0);
  free(w1);
  free(w2);
  fflush(stdout);
  return 0;
} /* main */

