#include <stdio.h>
#include <stdlib.h>

int main()
{   
	int workers_num = 0;
	int known_workers_num = 32;

	#pragma acc parallel num_gangs(8) num_workers(32)  
	{   
	    #pragma acc loop gang
	    for(int i=0; i<8; i++)
	    {
		    #pragma acc loop worker reduction(+:workers_num)
		    for(int j=0; j<32; j++)
		    {
			    workers_num++;
			}
		}
   }

	printf("Workers number: %d\n", workers_num);

  if(workers_num == known_workers_num)
  	printf("Success\n");
  else
  	printf("Failure\n");

}

