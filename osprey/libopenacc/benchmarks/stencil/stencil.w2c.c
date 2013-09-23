/*******************************************************
 * C file translated from WHIRL Sun Aug  4 10:49:34 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include <openacc_rtl.h>




static int read_data(
  float * A0,
  int nx,
  int ny,
  int nz,
  struct _IO_FILE * fp)
{
  
  int s;
  int i;
  int j;
  int k;
  
  s = 0;
  i = 0;
  while(nz > i)
  {
    _514 :;
    j = 0;
    while(ny > j)
    {
      _1026 :;
      k = 0;
      while(nx > k)
      {
        _1538 :;
        fread(A0 + (unsigned long long)((unsigned long long) s), 4ULL, 1ULL, fp);
        s = s + 1;
        k = k + 1;
        _1282 :;
      }
      goto _1794;
      _1794 :;
      j = j + 1;
      _770 :;
    }
    goto _2050;
    _2050 :;
    i = i + 1;
    _258 :;
  }
  goto _2306;
  _2306 :;
  return 0;
} /* read_data */


extern void cpu_stencil(
  float c0,
  float c1,
  float * A0,
  float * Anext,
  const int nx,
  const int ny,
  const int nz)
{
  
  float * __device_A0;
  float * __device_Anext;
  
  /*Begin_of_nested_PU(s)*/
  
  __accr_get_device_addr(A0, &__device_A0, 0U, (unsigned int)(nz * (nx * ny)) * 4U);
  __accr_get_device_addr(Anext, &__device_Anext, 0U, (unsigned int)(nz * (nx * ny)) * 4U);
  __accr_set_default_gang_vector();
  __accr_set_gang_num_y(255);
  __accr_set_gang_num_x(16);
  __accr_set_vector_num_z(2);
  __accr_set_vector_num_y(4);
  __accr_set_vector_num_x(64);
  __accr_push_kernel_param_scalar(&nx);
  __accr_push_kernel_param_scalar(&ny);
  __accr_push_kernel_param_scalar(&nz);
  __accr_push_kernel_param_scalar(&c1);
  __accr_push_kernel_param_pointer(&__device_A0);
  __accr_push_kernel_param_scalar(&c0);
  __accr_push_kernel_param_pointer(&__device_Anext);
  __accr_launchkernel("__accrg_cpu_stencil_1_1", "stencil.w2c.ptx");
  _514 :;
  _258 :;
  return;
} /* cpu_stencil */


extern void outputData(
  char * fName,
  float * h_A0,
  int nx,
  int ny,
  int nz)
{
  
  struct _IO_FILE * fid;
  unsigned int tmp32;
  struct _IO_FILE * _temp_call1;
  
  _temp_call1 = fopen(fName, "w\0");
  fid = _temp_call1;
  if((unsigned long long)(fid) == 0ULL)
  {
    fprintf(stderr, "Cannot open output file\n\0");
    exit(-1);
  }
  tmp32 = (unsigned int)(nz * (nx * ny));
  fwrite(&tmp32, 4ULL, 1ULL, fid);
  fwrite(h_A0, 4ULL, (unsigned long long) tmp32, fid);
  fclose(fid);
  return;
} /* outputData */


extern int main(
  int argc,
  char ** argv)
{
  
  register int _w2c___comma;
  register int _w2c___comma0;
  register int _w2c___comma1;
  register int _w2c___comma2;
  int nx;
  int ny;
  int nz;
  int size;
  int iteration;
  float c0;
  float c1;
  struct timeval tim;
  double start;
  double end;
  float * h_A0;
  float * h_Anext;
  float * temp;
  float * _temp__casttmp0;
  float * _temp__casttmp1;
  struct _IO_FILE * fp;
  int t;
  float * _temp_call2;
  float * _temp_call3;
  struct _IO_FILE * _temp_call4;
  int __acch_temp__is_pcreate;
  float * __device_h_Anext;
  float * __device_h_A0;
  
  /*Begin_of_nested_PU(s)*/
  
  printf("OpenACC-based 7 points stencil codes****\n\0");
  printf("Original version by Rengan Xu <uhxrg@cs.uh.edu>\n\0");
  printf("This version maintained by Rengan Xu  ***********\n\0");
  c0 = 1.6666667163e-01F;
  c1 = 2.7777777985e-02F;
  if(argc <= 4)
  {
    printf("Usage: probe nx ny nz tx ty t\nnx: the grid size x\nny: the grid size y\nnz: the grid size z\nt: the iteration time\n\0");
    return -1;
  }
  _w2c___comma = atoi(*(argv + 1LL));
  nx = _w2c___comma;
  if(nx <= 0)
  {
    return -1;
  }
  _w2c___comma0 = atoi(*(argv + 2LL));
  ny = _w2c___comma0;
  if(ny <= 0)
  {
    return -1;
  }
  _w2c___comma1 = atoi(*(argv + 3LL));
  nz = _w2c___comma1;
  if(nz <= 0)
  {
    return -1;
  }
  _w2c___comma2 = atoi(*(argv + 4LL));
  iteration = _w2c___comma2;
  if(iteration <= 0)
  {
    return -1;
  }
  size = nz * (nx * ny);
  _temp_call2 = malloc((unsigned long long)((unsigned long long) size) * 4ULL);
  _temp__casttmp0 = _temp_call2;
  h_A0 = _temp__casttmp0;
  _temp_call3 = malloc((unsigned long long)((unsigned long long) size) * 4ULL);
  _temp__casttmp1 = _temp_call3;
  h_Anext = _temp__casttmp1;
  _temp_call4 = fopen("/home/rengan/openacc-test-suite/certification_suite/PARBOIL/parboil/datasets/stencil/default/input/512x512x64x100.bin\0", "rb\0");
  fp = _temp_call4;
  read_data(h_A0, nx, ny, nz, fp);
  fclose(fp);
  memcpy(h_Anext, h_A0, (unsigned long long)((unsigned long long) size) * 4ULL);
  acc_init(1U);
  gettimeofday(&tim, (struct timezone *) 0ULL);
  start = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  __acch_temp__is_pcreate = __accr_present_create(h_Anext, 0U, (unsigned int) size, (unsigned int)(size) * 4U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(h_Anext, &__device_h_Anext, (unsigned int)(size) * 4U);
    __accr_memin_h2d(h_Anext, __device_h_Anext, (unsigned int)(size) * 4U, 0U);
  }
  __acch_temp__is_pcreate = __accr_present_create(h_A0, 0U, (unsigned int) size, (unsigned int)(size) * 4U);
  if(__acch_temp__is_pcreate == 0)
  {
    __accr_malloc_on_device(h_A0, &__device_h_A0, (unsigned int)(size) * 4U);
    __accr_memin_h2d(h_A0, __device_h_A0, (unsigned int)(size) * 4U, 0U);
  }
  t = 0;
  while(iteration > t)
  {
    _514 :;
    cpu_stencil(c0, c1, h_A0, h_Anext, nx, ny, nz);
    temp = h_A0;
    h_A0 = h_Anext;
    h_Anext = temp;
    t = t + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  __accr_memout_d2h(__device_h_A0, h_A0, (unsigned int)(size) * 4U, 0U);
  __accr_free_on_device(__device_h_Anext);
  __accr_free_on_device(__device_h_A0);
  _4098 :;
  gettimeofday(&tim, (struct timezone *) 0ULL);
  end = (double)((tim).tv_sec) + ((double)((tim).tv_usec) / 1.0e+06);
  outputData("out.dat\0", h_A0, nx, ny, nz);
  printf("The stencil execution time: %.2f s\n\0", end - start);
  free(h_A0);
  free(h_Anext);
  return 0;
} /* main */

