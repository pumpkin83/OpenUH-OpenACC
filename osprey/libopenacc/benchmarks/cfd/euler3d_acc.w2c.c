/*******************************************************
 * C file translated from WHIRL Wed Aug 28 10:41:24 2013
 *******************************************************/

/* Include file-level type and variable decls */
#include "euler3d_acc.w2c.h"


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


extern void dump(
  float * variables,
  int nel,
  int nelr)
{
  
  int i;
  int j;
  struct _IO_FILE * fh1;
  struct _IO_FILE * fh2;
  struct _IO_FILE * fh3;
  struct _IO_FILE * _temp_call0;
  struct _IO_FILE * _temp_call1;
  struct _IO_FILE * _temp_call2;
  
  _temp_call0 = fopen("density\0", "wt\0");
  fh1 = _temp_call0;
  fprintf(fh1, "%d %d\n\0", nel, nelr);
  i = 0;
  while(nel > i)
  {
    _514 :;
    fprintf(fh1, "%f\n\0", (double)(*(variables + (long long)(VAR_DENSITY + (i * NVAR)))));
    i = i + 1;
    _258 :;
  }
  goto _770;
  _770 :;
  fclose(fh1);
  _temp_call1 = fopen("momentum\0", "wt\0");
  fh2 = _temp_call1;
  fprintf(fh2, "%d %d\n\0", nel, nelr);
  i = 0;
  while(nel > i)
  {
    _1282 :;
    j = 0;
    while(j != NDIM)
    {
      _1794 :;
      fprintf(fh2, "%f \0", (double)(*(variables + (long long)((j + VAR_MOMENTUM) + (i * NVAR)))));
      j = j + 1;
      _1538 :;
    }
    goto _2050;
    _2050 :;
    fprintf(fh2, "\n\0");
    i = i + 1;
    _1026 :;
  }
  goto _2306;
  _2306 :;
  fclose(fh2);
  _temp_call2 = fopen("density_energy\0", "wt\0");
  fh3 = _temp_call2;
  fprintf(fh3, "%d %d\n\0", nel, nelr);
  i = 0;
  while(nel > i)
  {
    _2818 :;
    fprintf(fh3, "%f\n\0", (double)(*(variables + (long long)(VAR_DENSITY_ENERGY + (i * NVAR)))));
    i = i + 1;
    _2562 :;
  }
  goto _3074;
  _3074 :;
  fclose(fh3);
  return;
} /* dump */


extern void initialize_variables(
  int nelr,
  float * variables)
{
  
  int i;
  int j;
  
  i = 0;
  while(nelr > i)
  {
    _514 :;
    j = 0;
    while(j < NVAR)
    {
      _1026 :;
      * (variables + (long long)(j + (i * NVAR))) = (ff_variable)[j];
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
} /* initialize_variables */


__inline void compute_flux_contribution(
  float density,
  struct float3 * momentum,
  float density_energy,
  float pressure,
  struct float3 * velocity,
  struct float3 * fc_momentum_x,
  struct float3 * fc_momentum_y,
  struct float3 * fc_momentum_z,
  struct float3 * fc_density_energy)
{
  
  struct float3 _tmp0;
  float de_p;
  
  (fc_momentum_x) -> x = pressure + ((float)((struct float3)(momentum) -> x) * (float)((struct float3)(velocity) -> x));
  (fc_momentum_x) -> y = (float)((struct float3)(velocity) -> x) * (float)((struct float3)(momentum) -> y);
  (fc_momentum_x) -> z = (float)((struct float3)(velocity) -> x) * (float)((struct float3)(momentum) -> z);
  _tmp0 = (struct float3)(fc_momentum_x) -> y;
  (fc_momentum_y) -> x = *(float *) & _tmp0;
  (fc_momentum_y) -> y = pressure + ((float)((struct float3)(momentum) -> y) * (float)((struct float3)(velocity) -> y));
  (fc_momentum_y) -> z = (float)((struct float3)(velocity) -> y) * (float)((struct float3)(momentum) -> z);
  _tmp0 = (struct float3)(fc_momentum_x) -> z;
  (fc_momentum_z) -> x = *(float *) & _tmp0;
  _tmp0 = (struct float3)(fc_momentum_y) -> z;
  (fc_momentum_z) -> y = *(float *) & _tmp0;
  (fc_momentum_z) -> z = pressure + ((float)((struct float3)(momentum) -> z) * (float)((struct float3)(velocity) -> z));
  de_p = density_energy + pressure;
  (fc_density_energy) -> x = (float)((struct float3)(velocity) -> x) * de_p;
  (fc_density_energy) -> y = (float)((struct float3)(velocity) -> y) * de_p;
  (fc_density_energy) -> z = (float)((struct float3)(velocity) -> z) * de_p;
  return;
} /* compute_flux_contribution */


__inline void compute_velocity(
  float density,
  struct float3 * momentum,
  struct float3 * velocity)
{
  
  
  (velocity) -> x = (float)((struct float3)(momentum) -> x) / density;
  (velocity) -> y = (float)((struct float3)(momentum) -> y) / density;
  (velocity) -> z = (float)((struct float3)(momentum) -> z) / density;
  return;
} /* compute_velocity */


__inline float compute_speed_sqd(
  struct float3 * velocity)
{
  
  
  return(((float)((struct float3)(velocity) -> x) * (float)((struct float3)(velocity) -> x)) + ((float)((struct float3)(velocity) -> y) * (float)((struct float3)(velocity) -> y))) + ((float)((struct float3)(velocity) -> z) * (float)((struct float3)(velocity) -> z));
} /* compute_speed_sqd */


__inline float compute_pressure(
  float density,
  float density_energy,
  float speed_sqd)
{
  
  
  return((float)(GAMMA) + -1.0F) * (density_energy + (speed_sqd * (density * -5.0e-01F)));
} /* compute_pressure */


__inline float compute_speed_of_sound(
  float density,
  float pressure)
{
  
  float _temp___sqrt_arg0;
  float _temp___save_sqrt1;
  
  _temp___sqrt_arg0 = ((float)(GAMMA) * pressure) / density;
  _temp___save_sqrt1 = _F4SQRT(_temp___sqrt_arg0);
  if(_temp___save_sqrt1 != _temp___save_sqrt1)
  {
    _temp___save_sqrt1 = sqrtf(_temp___sqrt_arg0);
  }
  return _temp___save_sqrt1;
} /* compute_speed_of_sound */


extern void compute_step_factor(
  int nelr,
  float * variables,
  float * areas,
  float * step_factors)
{
  
  float * __device_variables;
  float * __device_areas;
  
  /*Begin_of_nested_PU(s)*/
  
  __accr_get_device_addr(variables, &__device_variables, 0U, (unsigned int)(nelr * NVAR) * 4U);
  __accr_get_device_addr(areas, &__device_areas, 0U, (unsigned int)(nelr) * 4U);
  __accr_set_default_gang_vector();
  __accr_set_gang_num_x(nelr / block_length);
  __accr_set_vector_num_x(block_length);
  __accr_push_kernel_param_scalar(&nelr);
  __accr_push_kernel_param_pointer(&__device_variables);
  __accr_push_kernel_param_scalar(&VAR_DENSITY);
  __accr_push_kernel_param_scalar(&NVAR);
  __accr_push_kernel_param_scalar(&VAR_MOMENTUM);
  __accr_push_kernel_param_scalar(&VAR_DENSITY_ENERGY);
  __accr_push_kernel_param_scalar(&GAMMA);
  __accr_push_kernel_param_pointer(&__device_areas);
  __accr_push_kernel_param_pointer(&step_factors);
  __accr_launchkernel("__accrg_compute_step_factor_1_1\0", "euler3d_acc.w2c.ptx\0", -2);
  _514 :;
  _258 :;
  return;
} /* compute_step_factor */

