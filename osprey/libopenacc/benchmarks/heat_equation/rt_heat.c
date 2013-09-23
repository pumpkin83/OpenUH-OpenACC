#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <openacc_rtl.h>

#define filename "out.dat"

// DIVIDE_INTO(x/y) for integers, used to determine # of blocks/warps etc.
#define DIVIDE_INTO(x,y) (((x) + (y) - 1)/(y))
// I2D to index into a linear memory space from a 2D array index pair

// Block size in the i and j directions

void step_kernel_cpu(
  int ni,
  int nj,
  double tfac,
  double* temp_in,
  double* temp_out)
{
	double *__device_temp_in;
	double *__device_temp_out;
	int block_x, block_y, threads_x, threads_y;
/*	
	threads_x = 64;
	threads_y = 8;

	block_x = DIVIDE_INTO(ni, threads_x);
	block_y = DIVIDE_INTO(nj, threads_y);
*/
	__accr_set_default_gang_vector();

	__accr_get_device_addr(temp_in, (void**)&__device_temp_in, 0, ni*nj*sizeof(double));
	__accr_get_device_addr(temp_out, (void**)&__device_temp_out, 0, ni*ni*sizeof(double));

//	__accr_set_gangs(block_x, block_y, 1);
//	__accr_set_vectors(threads_x, threads_y, 1);

	__accr_push_kernel_param_pointer((void**)&__device_temp_in);
	__accr_push_kernel_param_pointer((void**)&__device_temp_out);
	__accr_push_kernel_param_scalar(&ni);
	__accr_push_kernel_param_scalar(&nj);
	__accr_push_kernel_param_double(&tfac);

	__accr_launchkernel("step_kernel_gpu", "heat2.ptx");
	
}

int main(int argc, char** argv)
{
	int ni, nj, nstep;
	double tfac, *temp1_h, *temp2_h,  *dev_temp1, *dev_temp2, *temp_tmp;
	int i, j, i2d, istep;
	double temp_bl, temp_br, temp_tl, temp_tr;
	int is_pcreate;

	struct timeval tim;
	double start, end;
	FILE *fp;
	int fd;

	 // domain size and number of timesteps (iterations)
	ni = 1024;
	nj = 1024;
	nstep = 20000;
				     
	// allocate temperature array on host
	temp1_h = (double *)malloc(sizeof(double)*ni*nj);
	temp2_h = (double *)malloc(sizeof(double)*ni*nj);

	// initial temperature in interior
	for (j=1; j < nj-1; j++) {
		for (i=1; i < ni-1; i++) {
			i2d = i + ni*j;
			temp1_h[i2d] = 0.0f;
		}
	}

    // initial temperature on boundaries - set corners
    temp_bl = 200.0f;
    temp_br = 300.0f;
    temp_tl = 200.0f;
    temp_tr = 300.0f;

    // set edges by linear interpolation from corners
    for (i=0; i < ni; i++) {
        // bottom
        j = 0;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_bl + (temp_br-temp_bl)*(double)i/(double)(ni-1);

        // top
        j = nj-1;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_tl + (temp_tr-temp_tl)*(double)i/(double)(ni-1);
    }

    for (j=0; j < nj; j++) {
        // left
        i = 0;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_bl + (temp_tl-temp_bl)*(double)j/(double)(nj-1);
        
		// right
        i = ni-1;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_br + (temp_tr-temp_br)*(double)j/(double)(nj-1);
    }

    // duplicate temeperature array on host
    memcpy(temp2_h, temp1_h, sizeof(double)*ni*nj);
    
	tfac = 0.2f;
    
	gettimeofday(&tim, NULL);
	start = tim.tv_sec + (tim.tv_usec/1000000.0);

	__accr_setup();
	is_pcreate = __accr_present_create(temp2_h, 0, ni*nj, ni*nj*sizeof(double));
	if(is_pcreate == 0)
	{
		__accr_malloc_on_device(temp2_h, (void**)&dev_temp2, ni*nj*sizeof(double));
		__accr_memin_h2d(temp2_h, dev_temp2, ni*nj*sizeof(double), 0);
	}

	is_pcreate = __accr_present_create(temp1_h, 0, ni*nj, ni*nj*sizeof(double));
	if(is_pcreate == 0)
	{
		__accr_malloc_on_device(temp1_h, (void**)&dev_temp1, ni*nj*sizeof(double));
		__accr_memin_h2d(temp1_h, dev_temp1, ni*nj*sizeof(double), 0);
	}
	    

	for (istep=0; istep < nstep; istep++) {
		step_kernel_cpu(ni, nj, tfac, temp1_h, temp2_h);
	    
		// swap the temp pointers
        temp_tmp = temp1_h;
        temp1_h = temp2_h;
        temp2_h = temp_tmp;
    } 

	__accr_memout_d2h(dev_temp1, temp1_h, ni*nj*sizeof(double), 0);
	__accr_free_on_device(dev_temp1);
	__accr_free_on_device(dev_temp2);
	__accr_cleanup();

	gettimeofday(&tim, NULL);
    end = tim.tv_sec + (tim.tv_usec/1000000.0);
    printf("Time for computing: %.2f s\n",end-start);

#if 0
	fd = creat(filename, 00666);
	fd = open(filename, O_WRONLY);
	write(fd, temp1_h, (size_t)ni*nj*sizeof(double));
	close(fd);
#endif
#if 1
	 fp = fopen("out.dat", "w");
	 fprintf(fp, "%i %i\n", ni, nj);
	 for (j=0; j < nj; j++) {
	     for (i=0; i < ni; i++) {
	          fprintf(fp, "%d %d %f\n", j, i, temp1_h[i + ni*j]);
	      }
	 }
	 fclose(fp);
#endif	
}


