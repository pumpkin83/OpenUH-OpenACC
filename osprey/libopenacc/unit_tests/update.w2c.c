/*******************************************************
 * C file translated from WHIRL Wed Jun 12 18:00:20 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <openacc_rtl.h>


int main(
  int argc,
  const char ** argv)
{
  
  long long i;
  long long N;
  int error;
  double TEST_VAL;
  long long _temp___save_expr0;
  long long _temp___vla_bound1;
  unsigned long long _temp___save_expr2;
  double * X;
  void * _temp___alloca3;
  double(*X0)[];
  int __acch_temp__is_pcreate;
  double * __device_X;
  
  /*Begin_of_nested_PU(s)*/
  
  N = 1000000LL;
  TEST_VAL = 5.0;
  error = 0;
  _temp___save_expr0 = N;
  _temp___vla_bound1 = _temp___save_expr0 + -1LL;
  _temp___save_expr2 = (unsigned long long)(_temp___save_expr0) * 8ULL;
  _temp___save_expr2 = _temp___save_expr2;
  _temp___alloca3 = (void *)(alloca(0));
  X0 = (double(*)[])(alloca((unsigned long long)(_temp___save_expr0) * 8ULL));
  __accr_setup();
  __acch_temp__is_pcreate = __accr_present_create(X, 0, (int) N, (int)(N) * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(X, &__device_X, (int)(N) * 8);
    __accr_memin_h2d(X, __device_X, (int)(N) * 8, 0);
  }
  i = 0LL;
  while(i < N)
  {
    _514 :;
    (*X0)[i] = TEST_VAL;
    i = i + 1LL;
    _258 :;
  }
  goto _770;
  _770 :;
  __accr_update_device_variable(X, 0, (int)(N) * 8);
  __accr_set_default_gang_vector();
  __accr_push_kernel_param_pointer(&__device_X);
  __accr_push_kernel_param_scalar(&N);
  __accr_launchkernel("__accrg_main_1_1", "update.w2c.ptx");
  _4098 :;
  __accr_update_host_variable(X, 0, (int)(N) * 8);
  i = 0LL;
  while(i < N)
  {
    _1282 :;
    if((*X0)[i] != (TEST_VAL * 2.0))
    {
      error = error + 1;
    }
    i = i + 1LL;
    _1026 :;
  }
  goto _1538;
  _1538 :;
  __accr_memout_d2h(__device_X, X, (int)(N) * 8, 0);
  __accr_free_on_device(__device_X);
  __accr_cleanup();
  _3842 :;
  if(error == 0)
  {
    printf("TEST PASS!\n\0");
  }
  else
  {
    printf("TEST FAILED!\n\0");
  }
  return 0;
} /* main */

