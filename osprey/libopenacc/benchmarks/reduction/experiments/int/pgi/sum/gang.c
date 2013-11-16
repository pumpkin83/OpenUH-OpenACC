#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <openacc.h>
#include <sys/time.h>

int main()
{
	int i, j, k;
	int sum, known_sum;
	int NI, NJ, NK;
    int error;
	int *input, *temp;
    struct timeval tim;
    double start, end;
	
    NK = 1<<20;
	NJ = 2;
	NI = 32;
    
    error = 0;
	input = (int*)malloc(NK*NJ*NI*sizeof(int));
	temp = (int*)malloc(NK*NJ*NI*sizeof(int));
    
    acc_init(acc_device_default);
    
    srand((unsigned)time(0));
    
    for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
            {
				input[k*NJ*NI + j*NI + i] = rand()%5 + 1;
            }
		}
	}

    sum = 0;
    gettimeofday(&tim, NULL);
    start = tim.tv_sec*1000 + (tim.tv_usec/1000.0);
  #pragma acc parallel copyin(input[0:NK*NJ*NI]) \
  					   create(temp[0:NK*NJ*NI]) \
                       num_gangs(192) \
                       num_workers(8) \
                       vector_length(128) 
                    
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
    gettimeofday(&tim, NULL);
    end = tim.tv_sec*1000 + (tim.tv_usec/1000.0);

	known_sum = 0;
	for(k=0; k<NK; k++)
	{
		known_sum += input[k*NJ*NI];
	}

	if(sum != known_sum)
    {
        error++;
		printf("Int + FAILED! sum=%d, known_sum=%d\n", sum, known_sum);
    }
    printf("gang + execution time is :%.2lf: ms\n", end-start);

    if(error == 0)
        printf("gang + SUCCESS!\n");
	
    free(input);
	free(temp);
    return 0;
}
