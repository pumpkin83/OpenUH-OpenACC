#include <stdio.h>
#include <stdlib.h>
#include <openacc_rtl.h>

#define NK 8
#define NJ 16
#define NI 128
	
int x[NK][NJ][NI];
int y[NK][NJ][NI];

int main()
{
	int i, j, k;
	int error;

	for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
				x[k][j][i] = (k+j+i)%10;
		}
	}

	acc_init(acc_device_default);

//#pragma acc kernels copyin(x[0:NK][0:NJ][0:NI]) \
//					copyout(y[0:NK][0:NJ][0:NI])
#pragma acc kernels copyin(x) copyout(y)
{
	#pragma acc loop gang(8)
	//#pragma acc loop gang
	for(k=0; k<NK; k++)
	{
	   #pragma acc loop gang(16) vector(2)
	   //#pragma acc loop worker
		for(j=0; j<NJ; j++)
		{
		    #pragma acc loop vector(128)
			//#pragma acc loop vector
			for(i=0; i<NI; i++)
				y[k][j][i] = x[k][j][i]*3 + 2;
		}
	}
}	

	error = 0;
	for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
			{
				if(y[k][j][i] != x[k][j][i]*3 + 2)
					error ++;
			}
		}
	}

	if(error == 0)
		printf("Test success!\n");
	else
		printf("Test failed!\n");
		

	acc_shutdown(acc_device_default);
	return 0;
}
