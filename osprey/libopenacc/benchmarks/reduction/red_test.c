#include <stdio.h>
#include <stdlib.h>
#include <openacc_rtl.h>

void test1()
{
	int i, j, k;
	int sum, known_sum;
	int NI, NJ;
	int *input;

	NI = 2048;
	NJ = 1024;

	input = (int*)malloc(NI*NJ*sizeof(int));

	for(j=0; j<NJ; j++)
	{
		for(i=0; i<NI; i++)
			input[j*NI + i] = (j+i)%10;
	}

	sum = 0;
  #pragma acc kernels copyin(input[0:NI*NJ])
  {
	#pragma acc loop reduction(+:sum)
	for(j=0; j<NJ; j++)
	{
		int local_sum = 0;
		for(i=0; i<NI; i++)
			local_sum += input[j*NI + i];
		sum += local_sum;
	}
  }

	known_sum = 0;
	for(j=0; j<NJ; j++)
	{
		for(i=0; i<NI; i++)
			known_sum += input[j*NI + i];
	}

	free(input);
	if(sum == known_sum)
		printf("Success!\n");
	else
		printf("Failed! sum=%d, known_sum=%d\n", sum, known_sum);

}



int main()
{
	acc_init(acc_device_default);
	test1();
	acc_shutdown(acc_device_default);

	return 0;
}
