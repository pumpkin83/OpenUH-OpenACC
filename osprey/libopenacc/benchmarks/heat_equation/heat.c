#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#define DATATYPE double

#define filename "acc_single.dat"

// I2D to index into a linear memory space from a 2D array index pair
#define I2D(ni, i, j) ((i) + (ni)*(j))



// kernel to update temperatures - CPU version
void step_kernel_cpu(int ni, 
                     int nj,
                     DATATYPE tfac, 
                     DATATYPE *temp_in,
                     DATATYPE *temp_out) 
{
    int i, j, i00, im10, ip10, i0m1, i0p1;
    DATATYPE d2tdx2, d2tdy2;

    // loop over all points in domain (not boundary points)
    #pragma acc data present(temp_in[0:ni*nj], temp_out[0:ni*nj])
    {
		//#pragma acc kernels param(temp_in, temp_out, ni, nj, tfac)
		#pragma acc kernels
		{
			#pragma acc loop gang independent
			for (j=1; j < nj-1; j++) 
			{
				#pragma acc loop gang vector independent
				for (i=1; i < ni-1; i++) 
				{
					// find indices into linear memory for central point and neighbours
				    i00 = I2D(ni, i, j);
				    im10 = I2D(ni, i-1, j);
				    ip10 = I2D(ni, i+1, j);
				    i0m1 = I2D(ni, i, j-1);
				    i0p1 = I2D(ni, i, j+1);

					// evaluate derivatives
				    d2tdx2 = temp_in[im10] - 2*temp_in[i00] + temp_in[ip10];
				    d2tdy2 = temp_in[i0m1] - 2*temp_in[i00] + temp_in[i0p1];

					// update temperatures
				    temp_out[i00] = temp_in[i00] + tfac*(d2tdx2 + d2tdy2);
				}
			}
		}
	}
}

/*
// kernel to update temperatures - CPU version
void step_kernel_cpu(int ni, 
                     int nj,
                     float tfac, 
                     float *temp_in,
                     float *temp_out) {
    int i, j, i00, im10, ip10, i0m1, i0p1;
    float d2tdx2, d2tdy2;

    // loop over all points in domain (not boundary points)
#pragma acc kernels present(temp_in[0:ni*nj], temp_out[0:ni*nj]) param(temp_in, temp_out, ni, nj, tfac)
{
	#pragma acc loop gang independent private(i, j, i00, im10, ip10, i0m1, i0p1, d2tdx2, d2tdy2)
    for (j=1; j < nj-1; j++) 
    {
    	#pragma acc loop gang vector independent
        for (i=1; i < ni-1; i++) 
        {
	    	// find indices into linear memory for central point and neighbours
            i00 = I2D(ni, i, j);
            im10 = I2D(ni, i-1, j);
            ip10 = I2D(ni, i+1, j);
            i0m1 = I2D(ni, i, j-1);
            i0p1 = I2D(ni, i, j+1);

	    	// evaluate derivatives
            d2tdx2 = temp_in[im10] - 2*temp_in[i00] + temp_in[ip10];
            d2tdy2 = temp_in[i0m1] - 2*temp_in[i00] + temp_in[i0p1];

	    	// update temperatures
            temp_out[i00] = temp_in[i00] + tfac*(d2tdx2 + d2tdy2);
        }
    }

}
}*/


int main(int argc, char *argv[]) 
{
    int ni, nj, nstep;
    DATATYPE tfac, *temp1_h, *temp2_h, *temp_tmp;
    int i, j, i2d, istep;
    DATATYPE temp_bl, temp_br, temp_tl, temp_tr;
	struct timeval tim;
	double start, end;
    double time;
    FILE *fp;
	int fd;
   
    // domain size and number of timesteps (iterations)
    ni = 1024;
    nj = 1024;
    nstep = 20000;
    
    // allocate temperature array on host
    temp1_h = (DATATYPE *)malloc(sizeof(DATATYPE)*ni*nj);
    temp2_h = (DATATYPE *)malloc(sizeof(DATATYPE)*ni*nj);

    // initial temperature in interior
    for (j=1; j < nj-1; j++) 
    {
        for (i=1; i < ni-1; i++) 
        {
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
    for (i=0; i < ni; i++) 
    {
        // bottom
        j = 0;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_bl + (temp_br-temp_bl)*(DATATYPE)i/(DATATYPE)(ni-1);

        // top
        j = nj-1;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_tl + (temp_tr-temp_tl)*(DATATYPE)i/(DATATYPE)(ni-1);
    }

    for (j=0; j < nj; j++) 
    {
        // left
        i = 0;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_bl + (temp_tl-temp_bl)*(DATATYPE)j/(DATATYPE)(nj-1);

        // right
        i = ni-1;
        i2d = i + ni*j;
        temp1_h[i2d] = temp_br + (temp_tr-temp_br)*(DATATYPE)j/(DATATYPE)(nj-1);
    }

    // duplicate temeperature array on host
    memcpy(temp2_h, temp1_h, sizeof(DATATYPE)*ni*nj);
        
    tfac = 0.2f;
    
	gettimeofday(&tim, NULL);
	start = tim.tv_sec + (tim.tv_usec/1000000.0);

    // main iteration loop  

#pragma acc data copy(temp1_h[0:ni*nj]) \
copyin(temp2_h[0:ni*nj])
{
    for (istep=0; istep < nstep; istep++) 
    {
        // CPU kernel
        step_kernel_cpu(ni, nj, tfac, temp1_h, temp2_h);
            
            
		// kernel to update temperatures - CPU version
	    // swap the temp pointers
        temp_tmp = temp1_h;
        temp1_h = temp2_h;
        temp2_h = temp_tmp;
            
    } 
}
	gettimeofday(&tim, NULL);
    end = tim.tv_sec + (tim.tv_usec/1000000.0);
    printf("Time for computing: %.2f s\n",end-start);


    // output temp1 to a file
#if 1
    fp = fopen("single.dat", "w");
    fprintf(fp, "%i %i\n", ni, nj);
    for (j=0; j < nj; j++) {
        for (i=0; i < ni; i++) {
            fprintf(fp, "%d %d %f\n", j, i, temp1_h[i + ni*j]);
        }
    }
    fclose(fp);
#endif
#if 0
	fd = creat(filename, 00666);
	fd = open(filename, O_WRONLY);
	write(fd, temp1_h, ni*nj*sizeof(DATATYPE));
	close(fd);
#endif
}


