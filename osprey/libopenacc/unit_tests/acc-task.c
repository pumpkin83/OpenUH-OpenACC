#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <deque>

using namespace std;

// to handle asynchrouous tasks
pthread_t daemon_thread;
pthread_mutex_t lock_mutex;
pthread_cond_t work_todo_condvar;
int work_to_do;
int exit_signal;
typedef void (*pfunc)(void*);
deque<pfunc> task_queue;

void execute_task()
{
	
}

void execute_task_async(pfunc func)
{
    int status;

	status = pthread_mutex_lock(&lock_mutex);
    if(status != 0)
        printf("Error in locking mutex\n");
       
    task_queue.push_back(func);
    work_to_do = 1;
    
    status = pthread_cond_signal(&work_todo_condvar);

	printf("Added async task\n");
    
    status = pthread_mutex_unlock(&lock_mutex);
    if(status != 0)
        printf("Error in unlocking mutex\n");
}

void *perform_daemon_work(void* arg)
{
	int working;
    int status;

    deque<pfunc> pong_queue;

	working = 1;

    /*
      keep monitoring the status of the task queue until
      notified to exit
    */
    while(working)
    {
	    status = pthread_mutex_lock(&lock_mutex);
        if(status != 0)
            printf("Error in locking mutex\n");

	    while(!work_to_do){
            status = pthread_cond_wait(&work_todo_condvar, &lock_mutex); 		
            if(status != 0)
                printf("Error in condition wait\n");
	    }

        if(exit_signal)
            working = 0;
    
        pong_queue.swap(task_queue);
    
	    status = pthread_mutex_unlock(&lock_mutex);
        if(status != 0)
            printf("Error in unlocking mutex\n");

        while(!pong_queue.empty())
        {
            pong_queue.front()(NULL);
            printf("Executed async task\n");
            pong_queue.pop_front();
        }

    }
}

void func1(void* arg)
{
    printf("Executing func 1\n");
}

void func2(void* arg)
{
    printf("Executing func 2\n");
}

void func3(void* arg)
{
    printf("Executing func 3\n");
}

int main()
{
	pthread_mutex_init(&lock_mutex, NULL);
	pthread_cond_init(&work_todo_condvar, NULL);

	work_to_do = 1;
    exit_signal = 0;

	pthread_create(&daemon_thread, NULL, perform_daemon_work, (void*)NULL);

    pfunc p;
    p = func1;
    execute_task_async(p);

    p = func3;
    execute_task_async(p);
    
    p = func2;
    execute_task_async(p);

    exit_signal = 1;	
	pthread_join(daemon_thread, NULL);

	pthread_mutex_destroy(&lock_mutex);
	pthread_cond_destroy(&work_todo_condvar);

	return 1;
}
