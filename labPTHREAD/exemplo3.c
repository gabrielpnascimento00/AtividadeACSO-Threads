#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdio.h> 
#define NUM_THREADS 5

void *PrintHello(void *threadid) { 
	long tid; 
	tid = (long)threadid; 
	printf("Hello World! It's me, thread #%lu!\n", tid); 
        sleep(20);
	pthread_exit(NULL); 
} 

int main (int argc, char *argv[]) { 
	pthread_t threads[NUM_THREADS]; 
	int rc;
        long t; 
	for(t=0; t<NUM_THREADS; t++){ 
               printf("In main: creating thread  %lu\n", t); 
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t); 
		if (rc) { 
			printf("ERROR code is %d\n", rc); 
  			printf("Check PTHREAD_THREADS_MAX in /proc/sys/kernel/threads-max\n"); 
			exit(-1); 
		} 
	} 
     //  sleep(40);
       printf("In main: I have finished my job \n"); 
       pthread_exit(NULL);
       //return 0 
} 



