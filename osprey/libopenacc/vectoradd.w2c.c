/*******************************************************
 * C file translated from WHIRL Fri May 17 15:11:14 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc_rtl.h>


int main(
  int argc,
  char ** argv)
{
  
  register unsigned int _w2c___comma;
  register unsigned int _w2c___comma0;
  register unsigned int _w2c___comma1;
  int n;
  double * a;
  double * b;
  double * c;
  int bytes;
  int i;
  double sum;
  int __acch_temp__is_pcreate;
  double * __device_a;
  double * __device_b;
  double * __device_c;
  
  /*Begin_of_nested_PU(s)*/
  
  n = 10000;
  bytes = (int)((unsigned int) n * 8U);
  _w2c___comma = (unsigned int)(unsigned int) malloc((unsigned int) bytes);
  a = (double *) _w2c___comma;
  _w2c___comma0 = (unsigned int)(unsigned int) malloc((unsigned int) bytes);
  b = (double *) _w2c___comma0;
  _w2c___comma1 = (unsigned int)(unsigned int) malloc((unsigned int) bytes);
  c = (double *) _w2c___comma1;
  i = 0;
  while(n > i)
  {
    _514 :;
    //* (a + (unsigned int) i) = F8SIN((double)(i)) * F8SIN((double)(i));
    //* (b + (unsigned int) i) = F8COS((double)(i)) * F8COS((double)(i));
    * (a + (unsigned int) i) = sin((double)(i)) * sin((double)(i));
    * (b + (unsigned int) i) = cos((double)(i)) * cos((double)(i));
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  //__acch_temp__is_pcreate = __accr_present_create(a, 0, n * 8);
  __acch_temp__is_pcreate = __accr_present_create(a, 0, 0, n * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(a, &__device_a, n * 8);
    //__accr_memin_h2d(a, __device_a, n * 8, n * 8);
    __accr_memin_h2d(a, __device_a, n * 8);
  }
  //__acch_temp__is_pcreate = __accr_present_create(b, 0, n * 8);
  __acch_temp__is_pcreate = __accr_present_create(b, 0, 0, n * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(b, &__device_b, n * 8);
    //__accr_memin_h2d(b, __device_b, n * 8, n*8);
    __accr_memin_h2d(b, __device_b, n * 8);
  }
  //__acch_temp__is_pcreate = __accr_present_create(c, 0, n * 8);
  __acch_temp__is_pcreate = __accr_present_create(c, 0, 0, n * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(c, &__device_c, n * 8);
  }
  /*
  __accr_push_kernel_param_pointer(&__device_a);
  __accr_push_kernel_param_pointer(&__device_b);
  __accr_push_kernel_param_pointer(&__device_c);
  __accr_push_kernel_param_pointer(&n);
  */
  __accr_push_kernel_param_pointer(__device_a, ACC_KDATA_UINT8);
  __accr_push_kernel_param_pointer(__device_b, ACC_KDATA_UINT8);
  __accr_push_kernel_param_pointer(__device_c, ACC_KDATA_UINT8);
  __accr_push_kernel_param_int(&n);
	

	/*new added: setting gang and vector number*/
	__accr_set_gang_num_x(8);
	__accr_set_gang_num_y(1);
	__accr_set_gang_num_z(1);
	
	__accr_set_vector_num_x(256);
	__accr_set_vector_num_y(1);
	__accr_set_vector_num_z(1);

  //__accr_launchkernel("__accrg_main_1_1");
  __accr_launchkernel("_Z16__accrg_main_1_1PdS_S_i", "vectoradd.ptx");
  //__accr_memout_d2h(__device_c, c, n * 8, 0);
  __accr_memout_d2h(__device_c, c, n * 8);
  _2818 :;
  sum = 0.0;
  i = 0;
  while(n > i)
  {
    _1282 :;
    sum = *(c + (unsigned int) i) + sum;
    i = i + 1;
    _1026 :;
  }
  goto _1538;
  _1538 :;
  sum = sum / (double)(n);
  printf("final result: %f\n\0", sum);
  return 0;
} /* main */



