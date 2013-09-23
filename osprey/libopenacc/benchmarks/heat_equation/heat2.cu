
extern "C" __global__ void step_kernel_gpu(
  double* temp_in,
  double* temp_out,
  int ni,
  int nj,
  double tfac)
  {
	int i, j, ti, tj, i00, im10, ip10, i0m1, i0p1;
	double d2tdx2, d2tdy2;


/*
	if(i>0 && i<ni-1 && j>0 && j<nj-1)
	{
		// evaluate derivatives 
		d2tdx2 = temp_in[im10] - 2*temp_in[i00] + temp_in[ip10];
		d2tdy2 = temp_in[i0m1] - 2*temp_in[i00] + temp_in[i0p1];
										    	             
		// update temperature
		temp_out[i00] = temp_in[i00] + tfac*(d2tdx2 + d2tdy2);
	}
*/
	tj = threadIdx.y;
	j = blockIdx.y*blockDim.y + tj;
   while(j < nj)
   {
		ti = threadIdx.x;
		i = blockIdx.x*blockDim.x + ti;
		while(i < ni){
			if(i>0 && i<ni-1 && j>0 && j<nj-1)
			{
				i00 = j*ni + i;
				im10 = j*ni + (i-1);
				ip10 = j*ni + (i+1);
				i0m1 = (j-1)*ni + i;
				i0p1 = (j+1)*ni + i;

				d2tdx2 = temp_in[im10] - 2*temp_in[i00] + temp_in[ip10];
				d2tdy2 = temp_in[i0m1] - 2*temp_in[i00] + temp_in[i0p1];
				temp_out[i00] = temp_in[i00] + tfac*(d2tdx2 + d2tdy2);
			}
			i += blockDim.x * gridDim.x;
		}
		j += blockDim.y*gridDim.y;
	}
  }
