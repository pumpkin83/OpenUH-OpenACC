#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <openacc_rtl.h>

float get_sum(float* array, int n)
{
	int i, j;
	float sum = 0.0f;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			sum += array[i*n + j];
	}
	return sum;
}

int main(int argc, char** argv)
{
	int n1, n2;
	int i, j, k;
	unsigned int dev_bytes, host_bytes;
	float *a1, *b1, *c1;
	float *a2, *b2, *c2;
	float *__dev_a1, *__dev_b1, *__dev_c1;
	int __is_pcreate;
	int async_scalar;
	struct timeval tim;
	double start, end;
	float device_sum, host_sum;

	n1 = 8192;
	n2 = 1024;
	async_scalar = 9;
	
	srand(1234);
	dev_bytes = n1*n1*sizeof(float);
	a1 = (float*)valloc(dev_bytes);
	b1 = (float*)valloc(dev_bytes);
	c1 = (float*)valloc(dev_bytes);
	
	host_bytes = n2*n2*sizeof(float);
	a2 = (float*)malloc(host_bytes);
	b2 = (float*)malloc(host_bytes);
	c2 = (float*)malloc(host_bytes);
	
	for(i=0; i<n1; i++){
		for(j=0; j<n1; j++){
			a1[i*n1 + j] = rand()/(float)RAND_MAX;
			b1[i*n1 + j] = rand()/(float)RAND_MAX;
			c1[i*n1 + j] = 0.0f;
		}
	}
	
	for(i=0; i<n2; i++){
		for(j=0; j<n2; j++){
			a2[i*n2 + j] = rand()/(float)RAND_MAX;
			b2[i*n2 + j] = rand()/(float)RAND_MAX;
			c2[i*n2 + j] = 0.0f;
		}
	}

	acc_init(acc_device_not_host);
	
	gettimeofday(&tim, NULL);
	start = tim.tv_sec + (tim.tv_usec/1000000.0);
	
	__is_pcreate = __accr_present_create(a1, 0, n1*n1, n1*n1*sizeof(float));
	if(__is_pcreate == 0)
	{
		__accr_malloc_on_device(a1, &__dev_a1, n1*n1*sizeof(float));
//		__accr_memin_h2d(a1, __dev_a1, n1*n1*sizeof(float), 0);
	}

	__is_pcreate = __accr_present_create(b1, 0, n1*n1, n1*n1*sizeof(float));
	if(__is_pcreate == 0)
	{
		__accr_malloc_on_device(b1, &__dev_b1, n1*n1*sizeof(float));
//		__accr_memin_h2d(b1, __dev_b1, n1*n1*sizeof(float), 0);
	}

	__accr_update_device_variable_async(a1, 0, n1*n1*sizeof(float), async_scalar);
	__accr_update_device_variable_async(b1, 0, n1*n1*sizeof(float), async_scalar);
	
	__is_pcreate = __accr_present_create(c1, 0, n1*n1, n1*n1*sizeof(float));
	if(__is_pcreate == 0)
	{
		__accr_malloc_on_device(c1, &__dev_c1, n1*n1*sizeof(float));
	}

  __accr_set_default_gang_vector();
  __accr_set_gang_num_y(256);
  __accr_set_gang_num_x(256);
  __accr_set_vector_num_x(32);
  __accr_set_vector_num_y(32);
  __accr_push_kernel_param_pointer(&__dev_a1);
  __accr_push_kernel_param_pointer(&__dev_b1);
  __accr_push_kernel_param_pointer(&__dev_c1);
  __accr_push_kernel_param_scalar(&n1);
  __accr_launchkernel("async_kernel", "async_kernel.ptx", async_scalar);
//  __accr_launchkernel("async_kernel", "async_kernel.ptx");

  //__accr_memout_d2h(__dev_c1, c1, n1*n1*sizeof(float), 0U);
  __accr_update_host_variable_async(c1, 0, n1*n1*sizeof(float), async_scalar);

	for(i=0; i<n2; i++){
		for(j=0; j<n2; j++){
			float tmp = 0.0f;
			for(k=0; k<n2; k++)
				tmp += a2[i*n2 + k]*b2[k*n2 + j];
			c2[i*n2 + j] = tmp;
		}
	}

  __accr_wait_stream(async_scalar);

  __accr_free_on_device(__dev_a1);
  __accr_free_on_device(__dev_b1);
  __accr_free_on_device(__dev_c1);
	
  gettimeofday(&tim, NULL);
  end = tim.tv_sec + (tim.tv_usec/1000000.0);
  printf("Time for runtime async computing: %.2f s\n", end - start);

  device_sum = get_sum(c1, n1);
  host_sum = get_sum(c2, n2);
  printf("Device sum: %f\n", device_sum);
  printf("Host sum: %f\n", host_sum);

  acc_shutdown(acc_device_not_host);

  return 0;
}
