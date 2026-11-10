
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

void* function(void* arg) {
  printf( "This is thread %lu\n", (long) pthread_self() );
  sleep(5);
  /*while (1){
	  printf("x\n");
     sleep(1);

  }*/
  return (void *)99;
}
int main(void) {
  pthread_attr_t attr;
  pthread_t t2;
  void *result;
  pthread_attr_init( &attr );
  
  //primeiro comentando  o set  - ok / depois descomentando -- erro
  
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  
  pthread_create( &t2, &attr, function, NULL );
  int err = pthread_join(t2,&result);
 printf("Thread t2 returned %lu ... is there any error? err=%d (ESRCH=%d, EDEADLK=%d,EINVAL=%d)\n", (long) result,err, ESRCH, EDEADLK,EINVAL);
  return 0;
  //pthread_exit(NULL); 
}
