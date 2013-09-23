/*******************************************************
 * C file translated from WHIRL Tue May 21 19:11:49 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <openacc_rtl.h>

extern void step_kernel_cpu(
  int ni,
  int nj,
  double tfac,
  double * temp_in,
  double * temp_out)
{
  
  double * __device_temp_in;
  double * __device_temp_out;
  
  /*Begin_of_nested_PU(s)*/
  
  __accr_get_device_addr(temp_in, &__device_temp_in, 0, (ni * nj) * 8);
  __accr_get_device_addr(temp_out, &__device_temp_out, 0, (ni * nj) * 8);
  __accr_set_default_gang_vector();
  __accr_push_kernel_param_pointer(&__device_temp_in);
  __accr_push_kernel_param_pointer(&__device_temp_out);
  __accr_push_kernel_param_pointer(&ni);
  __accr_push_kernel_param_pointer(&nj);
  __accr_push_kernel_param_pointer(&tfac);
  __accr_launchkernel("__accrg_step_kernel_cpu_1_1", "heat.w2c.ptx");
  _258 :;
  return;
} /* step_kernel_cpu */


extern int main(
  int argc,
  char ** argv)
{
  
  register int _w2c___comma;
  register int _w2c___comma0;
  register int _w2c_reg1;
  int ni;
  int nj;
  int nstep;
  double tfac;
  double * temp1_h;
  double * temp2_h;
  double * temp_tmp;
  int i;
  int j;
  int i2d;
  int istep;
  double temp_bl;
  double temp_br;
  double temp_tl;
  double temp_tr;
  struct timeval tim;
  double start;
  double end;
  int fd;
  double * _temp__casttmp0;
  double * _temp__casttmp1;
  double * _temp_call5;
  double * _temp_call6;
  int __acch_temp__is_pcreate;
  double * __device_temp2_h;
  double * __device_temp1_h;
  
  /*Begin_of_nested_PU(s)*/
  
  ni = 8192;
  nj = 8192;
  nstep = 20000;
  _temp_call5 = malloc(((unsigned int) ni * (unsigned int) nj) * 8U);
  _temp__casttmp0 = _temp_call5;
  temp1_h = _temp__casttmp0;
  _temp_call6 = malloc(((unsigned int) ni * (unsigned int) nj) * 8U);
  _temp__casttmp1 = _temp_call6;
  temp2_h = _temp__casttmp1;
  j = 1;
  while(j < (nj + -1))
  {
    _514 :;
    i = 1;
    while(i < (ni + -1))
    {
      _1026 :;
      i2d = i + (ni * j);
      * (temp1_h + (unsigned int) i2d) = 0.0;
      i = i + 1;
      _770 :;
    }
    goto _1282;
    _1282 :;
    j = j + 1;
    _258 :;
  }
  goto _1538;
  _1538 :;
  temp_bl = 2.0e+02;
  temp_br = 3.0e+02;
  temp_tl = 2.0e+02;
  temp_tr = 3.0e+02;
  i = 0;
  while(ni > i)
  {
    _2050 :;
    j = 0;
    i2d = i + (ni * j);
    * (temp1_h + (unsigned int) i2d) = temp_bl + (((double)(i) * (temp_br - temp_bl)) / (double)((ni + -1)));
    j = nj + -1;
    i2d = i + (ni * j);
    * (temp1_h + (unsigned int) i2d) = temp_tl + (((double)(i) * (temp_tr - temp_tl)) / (double)((ni + -1)));
    i = i + 1;
    _1794 :;
  }
  goto _2306;
  _2306 :;
  j = 0;
  while(nj > j)
  {
    _2818 :;
    i = 0;
    i2d = i + (ni * j);
    * (temp1_h + (unsigned int) i2d) = temp_bl + (((double)(j) * (temp_tl - temp_bl)) / (double)((nj + -1)));
    i = ni + -1;
    i2d = i + (ni * j);
    * (temp1_h + (unsigned int) i2d) = temp_br + (((double)(j) * (temp_tr - temp_br)) / (double)((nj + -1)));
    j = j + 1;
    _2562 :;
  }
  goto _3074;
  _3074 :;
  memcpy(temp2_h, temp1_h, ((unsigned int) ni * (unsigned int) nj) * 8U);
  tfac = 2.0000000298023223877e-01;
  gettimeofday(&tim, (struct timezone *) 0U);
  start = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  __acch_temp__is_pcreate = __accr_present_create(temp2_h, 0, ni * nj, (ni * nj) * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(temp2_h, &__device_temp2_h, (ni * nj) * 8);
    __accr_memin_h2d(temp2_h, __device_temp2_h, (ni * nj) * 8, 0);
  }
  __acch_temp__is_pcreate = __accr_present_create(temp1_h, 0, ni * nj, (ni * nj) * 8);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(temp1_h, &__device_temp1_h, (ni * nj) * 8);
    __accr_memin_h2d(temp1_h, __device_temp1_h, (ni * nj) * 8, 0);
  }
  istep = 0;
  while(nstep > istep)
  {
    _3586 :;
    step_kernel_cpu(ni, nj, tfac, temp1_h, temp2_h);
    temp_tmp = temp1_h;
    temp1_h = temp2_h;
    temp2_h = temp_tmp;
    istep = istep + 1;
    _3330 :;
  }
  goto _3842;
  _3842 :;
  __accr_memout_d2h(__device_temp1_h, temp1_h, (ni * nj) * 8, 0);
  __accr_free_on_device(__device_temp2_h);
  __accr_free_on_device(__device_temp1_h);
  _6658 :;
  gettimeofday(&tim, (struct timezone *) 0U);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  printf("Time for computing: %.2f s\n\0", end - start);
  _w2c___comma = creat("acc_single.dat\0", 438U);
  fd = _w2c___comma;
  _w2c___comma0 = open("acc_single.dat\0", 1);
  fd = _w2c___comma0;
  write(fd, temp1_h, (unsigned int)((ni * nj)) * 8U);
  close(fd);
  return _w2c_reg1;
} /* main */

