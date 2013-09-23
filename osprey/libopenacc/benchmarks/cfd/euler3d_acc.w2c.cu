#define __nv50_blockIdx_x blockIdx.x
#define __nv50_blockIdx_y blockIdx.y
#define __nv50_blockIdx_z blockIdx.z
#define __nv50_threadIdx_x threadIdx.x
#define __nv50_threadIdx_y threadIdx.y
#define __nv50_threadIdx_z threadIdx.z
#define __nv50_blockdim_x blockDim.x
#define __nv50_blockdim_y blockDim.y
#define __nv50_blockdim_z blockDim.z
#define __nv50_griddim_x gridDim.x
#define __nv50_griddim_y gridDim.y
#define __nv50_griddim_z gridDim.z

static extern "C" __global__ void __accrg_compute_step_factor_1_1(
  int nelr,
  float * variables,
  int _w2c_VAR_DENSITY0,
  int _w2c_NVAR0,
  int _w2c_VAR_MOMENTUM0,
  int _w2c_VAR_DENSITY_ENERGY0,
  int _w2c_GAMMA0,
  float * areas,
  float * step_factors)
{
  
  int i;
  float density;
  struct float3 momentum;
  float density_energy;
  struct float3 velocity;
  float speed_sqd;
  float pressure;
  float _temp___sqrt_arg2;
  float _temp___save_sqrt3;
  float speed_of_sound;
  float _temp___sqrt_arg4;
  float _temp___save_sqrt5;
  float _temp___sqrt_arg6;
  float _temp___save_sqrt7;
  unsigned int __acc_tmp_0;
  
  i = __nv50_blockIdx_x * __nv50_blockdim_x;
  i = (unsigned int)(i) + __nv50_threadIdx_x;
  __acc_tmp_0 = __nv50_blockdim_x * __nv50_griddim_x;
  while(nelr > i)
  {
    if(i >= 0)
    {
      density = *(variables + (long long)(_w2c_VAR_DENSITY0 + (_w2c_NVAR0 * i)));
      (momentum).x = *(variables + (long long)(_w2c_VAR_MOMENTUM0 + (_w2c_NVAR0 * i)));
      (momentum).y = *(variables + (long long)((_w2c_VAR_MOMENTUM0 + (_w2c_NVAR0 * i)) + 1));
      (momentum).z = *(variables + (long long)((_w2c_VAR_MOMENTUM0 + (_w2c_NVAR0 * i)) + 2));
      density_energy = *(variables + (long long)(_w2c_VAR_DENSITY_ENERGY0 + (_w2c_NVAR0 * i)));
      (velocity).x = (momentum).x / density;
      (velocity).y = (momentum).y / density;
      (velocity).z = (momentum).z / density;
      speed_sqd = (((velocity).x * (velocity).x) + ((velocity).y * (velocity).y)) + ((velocity).z * (velocity).z);
      pressure = ((float)(_w2c_GAMMA0) + -1.0F) * (density_energy + (speed_sqd * (density * -5.0e-01F)));
      _temp___sqrt_arg2 = ((float)(_w2c_GAMMA0) * pressure) / density;
      _temp___save_sqrt3 = sqrtf(_temp___sqrt_arg2);
      speed_of_sound = _temp___save_sqrt3;
      _temp___sqrt_arg4 = *(areas + (unsigned long long)((unsigned long long) i));
      _temp___save_sqrt5 = sqrtf(_temp___sqrt_arg4);
      _temp___sqrt_arg6 = speed_sqd;
      _temp___save_sqrt7 = sqrtf(_temp___sqrt_arg6);
      * (step_factors + (unsigned long long)((unsigned long long) i)) = 5.0e-01F / (_temp___save_sqrt5 * (speed_of_sound + _temp___save_sqrt7));
    }
    i = i + (int)(__acc_tmp_0);
  }
} /* __accrg_compute_step_factor_1_1 */

