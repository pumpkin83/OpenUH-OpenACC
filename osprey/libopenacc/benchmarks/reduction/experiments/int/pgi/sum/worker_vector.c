
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <openacc.h>
#include <sys/time.h>

#define TYPE int

int main()
{
	int i, j, k;
	TYPE sum, known_sum;
	int NI, NJ, NK;
    int error;
	TYPE *input, *temp;
    struct timeval tim;
    double start, end;

	NK = 32;
	NJ = 1<<10;
	NI = 1<<10;

    error = 0;

	input = (TYPE*)malloc(NK*NJ*NI*sizeof(TYPE));
	temp = (TYPE*)malloc(NK*sizeof(TYPE));

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
  
    gettimeofday(&tim, NULL);
    start = tim.tv_sec*1000 + (tim.tv_usec/1000.0);
 #pragma acc parallel copyin(input[0:NK*NJ*NI]) \
  					  copyout(temp[0:NK]) 
  {
    #pragma acc loop gang
    for(k=0; k<NK; k++)
	{
        TYPE j_sum = 0;
		#pragma acc loop worker reduction(+:j_sum)
		for(j=0; j<NJ; j++)
		{
			#pragma acc loop vector
			for(i=0; i<NI; i++)
				j_sum += input[k*NJ*NI + j*NI + i];
		}
	    temp[k] = j_sum;
	}
  }
    gettimeofday(&tim, NULL);
    end = tim.tv_sec*1000 + (tim.tv_usec/1000.0);
	
    for(k=0; k<NK; k++)
	{
		TYPE j_sum = 0;
		for(j=0; j<NJ; j++)
		{
			for(i=0; i<NI; i++)
			    j_sum += input[k*NJ*NI + j*NI + i];
		}
		if(temp[k] != j_sum)
        {
			error++;
		    printf("worker_vector + FAILED!\n");
        }
	}
    
    printf("worker_vector + execution time is :%.2lf: ms\n", end-start);

    if(error == 0)
        printf("worker_vector + SUCCESS!\n");

    free(input);
    free(temp);
}
