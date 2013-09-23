#include <stdio.h>
#include <stdlib.h>

int main()
{   
	int N;
	int i, j, result;
	int *input;
	int sum, known_sum;

	N = 2048;
	input = (int*)malloc(N*sizeof(int));

	for(i=0; i<N; i++)
		input[i] = i;

	sum = 0;
  #pragma acc data copyin(input[0:N])
  {
	#pragma acc parallel num_gangs(8) \
						 num_workers(1) \
						 vector_length(1) \
						 reduction(+:sum)
	//#pragma acc parallel reduction(+:sum)
	{
		#pragma acc loop gang
		for(i=0; i<N; i++){
			sum += input[i];
		}
	}
  }

	free(input);
	known_sum = N*(N-1)/2;


  if(sum == known_sum)
  	printf("Success\n");
  else
  	printf("Failure, sum=%d, known_sum=%d\n", sum, known_sum);

}

