#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	double *data;
	int i, N;
	double sum;

	N = 2048;
	data = (double*)malloc(N*sizeof(double));

	for(i=0; i<N; i++)
		data[i] = (double)i;

	sum = 0.0;
	#pragma acc data copyin(data[0:N])
	{
		#pragma acc kernels param(data, N)
		{
			#pragma acc loop reduction(+:sum)
			for(i=0; i<N; i++)
			{
				sum += data[i];
			}
		}
	}

	printf("Reduction sum is: %f\n", sum);

	return 1;
}
