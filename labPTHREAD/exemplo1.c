#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

// Insert the -lpthread option to compile (note the position of the option) 
// gcc exemplo1.c -o exemplo1 -lpthread


int global;
void *thr_func(void *arg);
int main(void)
{
	pthread_t tid, tid2;   // variable to store the thread ID
	global = 20;    //variable to be handled by the threads
	printf("Thread principal: %d\n", global);  // until here, there is only the main thread
	
	pthread_create(&tid, NULL, thr_func, NULL); // a new thread is created and it will execute the thr_func function 
	pthread_create(&tid2, NULL, thr_func, NULL);
	
	pthread_join(tid, NULL); // the main thread waits for the new thread to finish its job
       pthread_join(tid2, NULL); // the main thread waits for the new thread to finish its job
        //sleep(1);
	
	printf("Thread principal: %d\n", global); //From here, there is only the main thread
	return 0;
}

void *thr_func(void *arg)
{
	global = global+ 40;
	printf("Nova thread: %d\n", global);
        sleep(10);
        printf("END Nova thread\n");
	return NULL;
}

