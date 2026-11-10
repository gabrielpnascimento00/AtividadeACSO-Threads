#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_attr_t attr;

void* function(void* arg) {
  printf( "This is thread %d, PID %d \n", (int) pthread_self(),getpid() );
  sleep(5);
  return (void *)99;
}
int main(void) {
  printf( "This is the main thread %d, PID %d \n", (int) pthread_self(),getpid() );
  pthread_t t2;
  void *result;
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  pthread_create( &t2, &attr, function, NULL );
  //pthread_join(t2,&result);
 
 // int ret = pthread_attr_destroy(&attr);
 
  printf("Thread t2 returned %d\n", (int) result);
  pthread_exit(0);
  //return 0;
}
