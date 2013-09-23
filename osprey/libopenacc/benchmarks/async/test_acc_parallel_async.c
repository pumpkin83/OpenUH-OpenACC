#include "acc_testsuite.h"
#include <omp.h>
#ifdef _OPENACC
	#include <openacc.h>
#endif

int test_acc_parallel_async(FILE* logFile)
{
	int N = 1000;
	int A[N], B[N], C[N];
	int is_sync = -1;
	int result = 0;

	/*initialization*/
	for(int i=0; i<N; i++)
	{
	  A[i] = i;
	  B[i] = i;
	  C[i] = 0;
	}
	/*test case 1: check the #pragma acc parallel async() with arguments*/
 #pragma acc data copyin(A[0:N], B[0:N]) copy(C[0:N])
 {
	#pragma acc parallel loop async(1)
	  for(int i=0; i<N; i++)
	  {
	    C[i] = A[i] + B[i];
	  }
	
	is_sync = acc_async_test(1); //should be zero, since gpu has not done the computation.
	
	if(is_sync != 0)
	  result++;

	#pragma acc wait(1)

	is_sync = acc_async_test(1); //should be non-zero, since there is a "barrier" before
	if(is_sync == 0)
		result++;
}

    /*test case 2: check #pragma acc parallel async without arguments, in this case,*/
    /*the implementation will use a value distinct from all explict async argu in the program*/
 #pragma acc data copyin(A[0:N], B[0:N]) copy(C[0:N])
 {
    #pragma acc parallel loop async
	  for(int i=0; i<N; i++)
	  {
	    C[i] = A[i] + B[i];
	  }
		
	is_sync = acc_async_test_all(); //should be zero, since gpu has not done the computation.
	
	if(is_sync != 0)
	  result++;

	#pragma acc wait
	
	is_sync = acc_async_test_all(); //should be non-zero, since there is a "barrier" before
	if(is_sync == 0)
	  result++;
}
	/*test case 3: test for the case more host threads sharings the same accelator, sepc page:30*/
	/*test when two threads share the same async value, but should not iterfere with each other*/
	/*
	#pragma omp parallel for num_threads(2) firstprivate(is_sync) reduction(+:result)
	for(int i=0; i<omp_get_num_threads(); i++)
	{
	  if(omp_get_thread_num()==0)
	  {
	    #pragma acc parallel copyin(N, A[0:N]) async(omp_get_num_threads())
		  for(int j=0; j<N; j++)
		  {
		    A[j] = A[j] + 1;
		  }
	  }
	  
      if(omp_get_thread_num()==1)
	  {
	    #pragma acc parallel copyin(N,B[0:N]) async(omp_get_num_threads())
		  for(int k=0; k<N; k++)
		  {
		    B[k] = B[k] + 1;
		  }
	  }
		
	  is_sync = acc_async_test(omp_get_num_threads()); //should be zero, since async has not been done.
	  if(is_sync != 0)
	  {
	    result++;
	  }
		
	  #pragma acc wait(omp_get_num_threads())
		
	  is_sync = acc_async_test(omp_get_num_threads());
	  if(is_sync == 0)	//should be non-zero, since there is a wait before.
	  {
	    result++;
	  }
	}
	*/
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
	static const char * logFileName = "test_acc_parallel_async.log";	/* name of the logfile */


	/* Open a new Logfile or overwrite the existing one. */
	logFile = fopen(logFileName,"w+");

	printf("######## OpenACC Validation Suite V %s ######\n", ACCTS_VERSION );
	printf("## Repetitions: %3d                       ####\n",REPETITIONS);
	printf("## Loop Count : %6d                    ####\n",LOOPCOUNT);
	printf("##############################################\n");
	printf("Testing acc_parallel_async\n\n");

	fprintf(logFile,"######## OpenACC Validation Suite V %s ######\n", ACCTS_VERSION );
	fprintf(logFile,"## Repetitions: %3d                       ####\n",REPETITIONS);
	fprintf(logFile,"## Loop Count : %6d                    ####\n",LOOPCOUNT);
	fprintf(logFile,"##############################################\n");
	fprintf(logFile,"Testing acc_parallel_async\n\n");

	for ( i = 0; i < REPETITIONS; i++ ) {
		fprintf (logFile, "\n\n%d. run of test_acc_parallel_async out of %d\n\n",i+1,REPETITIONS);
		if(test_acc_parallel_async(logFile)){
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
