#include <stdio.h>
#include <stdlib.h>

int main()
{   
	int gangs, gangs_load;
	int workers, workers_load;
	int *gangs_red;
	int i, j, result;

	gangs = 8;
	workers = 16;
	workers_load = 32;
	gangs_red = (int*)malloc(gangs*sizeof(int));
	for(i=0; i<gangs; i++)
		gangs_red[i] = 0;

	#pragma acc parallel copy(gangs_red[0:gangs]) \
					num_gangs(gangs) num_workers(workers)
	{   
	    #pragma acc loop gang
	    for(i=0; i<gangs; i++){
			int to_reduct = 0;
		    #pragma acc loop worker reduction(+:to_reduct)
		    for(j=0; j<workers_load; j++)
			    to_reduct++;
			gangs_red[i] = to_reduct;
		}
   }

	result = 0;
	for(i=0; i<gangs; i++){
		if(gangs_red[i] != workers_load){
			result++;
			printf("workers[%d]=%d\n", i, gangs_red[i]);
		}
	}

	free(gangs_red);

  if(result == 0)
  	printf("Success\n");
  else
  	printf("Failure, result=%d\n", result);

}

