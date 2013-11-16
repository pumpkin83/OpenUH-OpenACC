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
	int sum, known_sum;
	int NI, NJ, NK;
    int error;
	TYPE *input, *temp;
    struct timeval tim;
    double start, end;
	
	NK = 2;
	NJ = 32;
    NI = 1<<20;
    
    error = 0;
	input = (TYPE*)malloc(NK*NJ*NI*sizeof(TYPE));
	temp = (TYPE*)malloc(NK*NJ*NI*sizeof(TYPE));
    
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
  					   copyout(temp[0:NK*NJ*NI]) \
                       num_gangs(192) \
                       num_workers(8) \
                       vector_length(128) 
  {
    #pragma acc loop gang
    for(k=0; k<NK; k++)
	{
		#pragma acc loop worker
		for(j=0; j<NJ; j++)
		{
            TYPE product = 1;
			#pragma acc loop vector reduction(*:product)
			for(i=0; i<NI; i++)
				product *= input[k*NJ*NI + j*NI + i];
            temp[k*NJ*NI + j*NI] = product;
		}
	}
  }
    gettimeofday(&tim, NULL);
    end = tim.tv_sec*1000 + (tim.tv_usec/1000.0);

	for(k=0; k<NK; k++)
	{
		for(j=0; j<NJ; j++)
		{
            TYPE product = 1;
			for(i=0; i<NI; i++)
				product *= input[k*NJ*NI + j*NI + i];

            if(temp[k*NJ*NI + j*NI] != product)
            {
			    error++;
		        printf("vecotr * FAILED!\n");
            }
		}
	}
    
    printf("vector * execution time is :%.2lf: ms\n", end-start);
    if(error == 0)
        printf("vector * SUCCESS!\n");
    
    free(input);
    free(temp);
}
