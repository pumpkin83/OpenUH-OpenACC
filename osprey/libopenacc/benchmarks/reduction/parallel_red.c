#include <stdio.h>
#include <stdlib.h>

void reduction10()
{
	int i, j, k;
	int sum, known_sum;
	int NI, NJ;
	int *input, *temp;

	NI = 2048;
	NJ = 1024;

	input = (int*)malloc(NI*NJ*sizeof(int));
	temp = (int*)malloc(NI*NJ*sizeof(int));

	for(j=0; j<NJ; j++)
	{
		for(i=0; i<NI; i++)
			input[j*NI + i] = (j+i)%10;
	}

	sum = 0;
  #pragma acc parallel copyin(input[0:NJ*NI]) \
  					   create(temp[0:NJ*NI])
  {
	#pragma acc loop gang reduction(+:sum)
	for(j=0; j<NJ; j++)
	{
		#pragma acc loop vector 
		for(i=0; i<NI; i++)
			temp[j*NI + i] = input[j*NI + i];
		sum += temp[j*NI + 0];
	}
  }

	known_sum = 0;
	for(j=0; j<NJ; j++)
	{
		known_sum += input[j*NI + 0];
	}

	free(input);
	free(temp);
	
	printf("Test for reduction 10\n");
	if(sum == known_sum)
		printf("Success!\n");
	else
		printf("Failed! sum=%d, known_sum=%d\n", sum, known_sum);
}

void reduction01()
{
	int i, j, k;
	int sum, known_sum;
	int NI, NJ;
	int *input, *temp;
	int error;

	NI = 2048;
	NJ = 1024;

	input = (int*)malloc(NI*NJ*sizeof(int));
	temp = (int*)malloc(NJ*sizeof(int));

	for(j=0; j<NJ; j++)
	{
		for(i=0; i<NI; i++)
			input[j*NI + i] = (j+i)%10;
	}

	sum = 0;
  #pragma acc parallel copyin(input[0:NJ*NI]) \
  					   copyout(temp[0:NJ])
  {
	#pragma acc loop gang
	for(j=0; j<NJ; j++)
	{
		int j_sum = 0;
		#pragma acc loop vector reduction(+:j_sum)
		for(i=0; i<NI; i++)
			j_sum += input[j*NI + i];
		temp[j] = j_sum;
	}
  }

	error = 0;
	for(j=0; j<NJ; j++)
	{
		int j_sum = 0;
		for(i=0; i<NI; i++)
			j_sum += input[j*NI + i];
		if(temp[j] != j_sum)
			error++;
	}

	free(input);
	free(temp);
	
	printf("Test for reduction 01\n");
	if(error == 0)
		printf("Success!\n");
	else
		printf("Failed! error=%d\n", error);
}

void reduction11()
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
  #pragma acc parallel copyin(input[0:NI*NJ])
  {
	#pragma acc loop gang reduction(+:sum)
	for(j=0; j<NJ; j++)
	{
		int local_sum = 0;
		#pragma acc loop vector reduction(+:local_sum)
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

void reduction111()
{
	int i, j, k;
	int sum, known_sum;
	int NI, NJ, NK;
	int *input;

	NI = 1024;
	NJ = 512;
	NK = 128;

	input = (int*)malloc(NK*NJ*NI*sizeof(int));

	for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
				input[k*NJ*NI + j*NI + i] = (k+j+i)%10;
		}
	}

	sum = 0;
  #pragma acc parallel copyin(input[0:NK*NJ*NI])
  {
    #pragma acc loop gang reduction(+:sum)
    for(k=0; k<NK; k++)
	{
		int j_sum = 0;
		#pragma acc loop worker reduction(+:j_sum)
		for(j=0; j<NJ; j++)
		{
			int i_sum = 0;
			#pragma acc loop vector reduction(+:i_sum)
			for(i=0; i<NI; i++)
				i_sum += input[k*NJ*NI + j*NI + i];
			j_sum += i_sum;
		}
		sum += j_sum;
	}
  }

	known_sum = 0;
	for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
				known_sum += input[k*NJ*NI + j*NI + i];
		}
	}

	free(input);
	if(sum == known_sum)
		printf("Success!\n");
	else
		printf("Failed! sum=%d, known_sum=%d\n", sum, known_sum);

}

void reduction100()
{
	int i, j, k;
	int sum, known_sum;
	int NI, NJ, NK;
	int *input, *temp;

	NI = 1024;
	NJ = 512;
	NK = 128;

	input = (int*)malloc(NK*NJ*NI*sizeof(int));
	temp = (int*)malloc(NK*NJ*NI*sizeof(int));

	for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
				input[k*NJ*NI + j*NI + i] = (k+j+i)%10;
		}
	}

	sum = 0;
  #pragma acc parallel copyin(input[0:NK*NJ*NI]) \
  					   create(temp[0:NK*NJ*NI])
  {
    #pragma acc loop gang reduction(+:sum)
    for(k=0; k<NK; k++)
	{
		#pragma acc loop worker
		for(j=0; j<NJ; j++)
		{
			#pragma acc loop vector
			for(i=0; i<NI; i++)
				temp[k*NJ*NI + j*NI + i] = input[k*NJ*NI + j*NI + i];
		}
		sum += temp[k*NJ*NI];
	}
  }

	known_sum = 0;
	for(k=0; k<NK; k++)
	{
		known_sum += input[k*NJ*NI];
	}

	free(input);
	free(temp);
	printf("Test for reduction 100\n");
	if(sum == known_sum)
		printf("Success!\n");
	else
		printf("Failed! sum=%d, known_sum=%d\n", sum, known_sum);

}

#if 0
void reduction010()
{
	int i, j, k;
	int sum, known_sum;
	int NI, NJ, NK;
	int *input, *temp;

	NI = 1024;
	NJ = 512;
	NK = 128;

	input = (int*)malloc(NK*NJ*NI*sizeof(int));
	temp = (int*)malloc(NK*NJ*NI*sizeof(int));

	for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
				input[k*NJ*NI + j*NI + i] = (k+j+i)%10;
		}
	}

	sum = 0;
  #pragma acc parallel copyin(input[0:NK*NJ*NI]) \
  					   copout(temp[0:NK*NJ*NI])
  {
    #pragma acc loop gang
    for(k=0; k<NK; k++)
	{
		int j_sum = 0;
		#pragma acc loop worker reduction(+:j_sum)
		for(j=0; j<NJ; j++)
		{
			#pragma acc loop vector
			for(i=0; i<NI; i++)
				temp[k*NJ*NI + j*NI + i] = input[k*NJ*NI + j*NI + i];
			j_sum += temp[k*NJ*NI + j*NI + 0];
		}
		temp[k*NJ*NI + 0 + 0] = j_sum;
	}
  }

	known_sum = 0;
	for(k=0; k<NK; k++)
	{
		known_sum += input[k*NJ*NI];
	}

	free(input);
	free(temp);
	printf("Test for reduction 010\n");
	if(sum == known_sum)
		printf("Success!\n");
	else
		printf("Failed! sum=%d, known_sum=%d\n", sum, known_sum);

}
#endif
int main()
{
	//reduction10();
	//reduction01();
	//reduction22();
	reduction111();
	//reduction100();

	return 0;
}
