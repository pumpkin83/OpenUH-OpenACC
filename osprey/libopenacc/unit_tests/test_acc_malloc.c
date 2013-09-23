
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openacc.h>

#define ACCTS_VERSION "1.0a"
#define LOOPCOUNT   1000
#define REPETITIONS 50
/* following times are in seconds */
#define SLEEPTIME    0.01
#define SLEEPTIME_LONG   0.5


int test_acc_malloc(FILE* logFile)
{
	int i, result = 0;
	int N = 1000;
	int A[N];
	int *array=NULL;
	
	/*initialization*/
	for(i=0; i<N; i++)
		A[i] = 0;

	array=(int*)acc_malloc(sizeof(int)*N);
	#pragma acc data deviceptr(array[0:N])
	{
		#pragma acc kernels copy(A[0:N])
		{
			#pragma acc loop
				for(i=0; i<N; i++)
					array[i] = i;

			#pragma acc loop
				for(i=0; i<N; i++)
				{
					array[i] = array[i] + 1;
					A[i] = array[i];
				}
		}
	}
	acc_free(array);

	for(i=0; i<N; i++)
	{
		if(A[i] != i + 1)
			result++;
	}

	if(result == 0)
		return 1;
	else
		return 0;

}

int main()
{
	int i;			/* Loop index */
	int result;		/* return value of the program */
	int failed=0; 		/* Number of failed tests */
	int success=0;		/* number of succeeded tests */
	static FILE * logFile;	/* pointer onto the logfile */
	static const char * logFileName = "test_acc_malloc.log";	/* name of the logfile */


	/* Open a new Logfile or overwrite the existing one. */
	logFile = fopen(logFileName,"w+");

	printf("######## OpenACC Validation Suite V %s ######\n", ACCTS_VERSION );
	printf("## Repetitions: %3d                       ####\n",REPETITIONS);
	printf("## Loop Count : %6d                    ####\n",LOOPCOUNT);
	printf("##############################################\n");
	printf("Testing acc_malloc\n\n");

	fprintf(logFile,"######## OpenACC Validation Suite V %s ######\n", ACCTS_VERSION );
	fprintf(logFile,"## Repetitions: %3d                       ####\n",REPETITIONS);
	fprintf(logFile,"## Loop Count : %6d                    ####\n",LOOPCOUNT);
	fprintf(logFile,"##############################################\n");
	fprintf(logFile,"Testing acc_malloc\n\n");

	for ( i = 0; i < REPETITIONS; i++ ) {
		fprintf (logFile, "\n\n%d. run of test_acc_malloc out of %d\n\n",i+1,REPETITIONS);
		if(test_acc_malloc(logFile)){
			fprintf(logFile,"Test successful.\n");
			success++;
		}
		else {
			fprintf(logFile,"Error: Test failed.\n");
			printf("Error: Test failed.\n");
			failed++;
		}
	}

    if(failed==0){
		fprintf(logFile,"\nDirective worked without errors.\n");
		printf("Directive worked without errors.\n");
		result=0;
	}
	else{
		fprintf(logFile,"\nDirective failed the test %i times out of %i. %i were successful\n",failed,REPETITIONS,success);
		printf("Directive failed the test %i times out of %i.\n%i test(s) were successful\n",failed,REPETITIONS,success);
		result = (int) (((double) failed / (double) REPETITIONS ) * 100 );
	}
	printf ("Result: %i\n", result);
	return result;
}
