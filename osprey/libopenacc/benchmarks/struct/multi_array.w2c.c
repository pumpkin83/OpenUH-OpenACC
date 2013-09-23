/*******************************************************
 * C file translated from WHIRL Sat Sep  7 23:59:11 2013
 *******************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<openacc_rtl.h>



extern int main()
{
  
  int i;
  int j;
  int k;
  int error;
  int __acch_temp__is_pcreate;
  int * __device_x;
  int * __device_y;
  
  /*Begin_of_nested_PU(s)*/
  
  k = 0;
  while(k <= 7)
  {
    _514 :;
    j = 0;
    while(j <= 15)
    {
      _1026 :;
      i = 0;
      while(i <= 127)
      {
        _1538 :;
        (x)[k * 16 * 128 + j * 128 + i] = (i + (j + k)) % 10;
        i = i + 1;
        _1282 :;
      }
      goto _1794;
      _1794 :;
      j = j + 1;
      _770 :;
    }
    goto _2050;
    _2050 :;
    k = k + 1;
    _258 :;
  }
  goto _2306;
  _2306 :;
  acc_init(1U);
  __acch_temp__is_pcreate = __accr_present_create(x, 0U, (unsigned int) 65536, 65536U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(x, &__device_x, 65536U);
    __accr_memin_h2d(x, __device_x, 65536U, 0U, -2);
  }
  __acch_temp__is_pcreate = __accr_present_create(y, 0U, (unsigned int) 65536, 65536U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(y, &__device_y, 65536U);
  }
  __accr_set_default_gang_vector();
  __accr_set_gang_num_x(8);
  __accr_set_gang_num_y(16);
  __accr_set_vector_num_y(2);
  __accr_set_vector_num_x(128);
  __accr_push_kernel_param_pointer(&__device_x);
  __accr_push_kernel_param_pointer(&__device_y);
  __accr_launchkernel("__accrg_main_1_1\0", "multi_array.w2c.ptx\0", -2);
  __accr_memout_d2h(__device_y, y, 65536U, 0U, -2);
  __accr_free_on_device(__device_x);
  __accr_free_on_device(__device_y);
  _8962 :;
  error = 0;
  k = 0;
  while(k <= 7)
  {
    _2818 :;
    j = 0;
    while(j <= 15)
    {
      _3330 :;
      i = 0;
      while(i <= 127)
      {
        _3842 :;
        if((y)[k * 16 * 128 + j * 128 + i] != (((x)[k * 16 * 128 + j * 128 + i] * 3) + 2))
        {
          error = error + 1;
        }
        i = i + 1;
        _3586 :;
      }
      goto _4098;
      _4098 :;
      j = j + 1;
      _3074 :;
    }
    goto _4354;
    _4354 :;
    k = k + 1;
    _2562 :;
  }
  goto _4610;
  _4610 :;
  if(error == 0)
  {
    printf("Test success!\n\0");
  }
  else
  {
    printf("Test failed!\n\0");
  }
  acc_shutdown(1U);
  return 0;
} /* main */

