/* mutualExclusion.c demonstrates the Mutual Exclusion pattern using pthreads
 *  to deposit and withdraw $$ from a bank account.
 *  When an even number of threads are used, the final balance should be $0.00,
 *   but this does not happen, due to a race condition.
 *
 * Joel Adams, Calvin College, Fall 2013.
 *
 * Usage: ./mutualExclusion [evenNumberOfThreads]
 * 
 * Exercise:
 *   1. Build and run several times, for each of 2, 4, 8, 16, ... threads.
 *   2. In bankAccount.h:
 *      - Surround each nonlocal access to bankAccountBalance with:
 *             pthread_mutex_lock( &lock );   // before the access
 *             pthread_mutex_unlock( &lock ); // after the access
 *   3. Save, rebuild, rerun, and note the difference in results.
 */

#define _POSIX_C_SOURCE 200809L //Para liberar o uso do rand_r (com seed)

#include <stdio.h>        // printf()
#include <stdlib.h>       // atoi(), exit(), ...
#include <pthread.h>      // pthread types and functions
#include <time.h>         // biblioteca normalmente utilizada para escolher aleatoriamente (usando uma seed única determinada pelo relogio atual)

#include "bankAccount.h"  // bank account info

void Operacoes (unsigned long id){
   unsigned int seed = time(NULL) + id;

   for (int i = 0; i < 1000; i++){
      int operacao = rand_r(&seed) % 4; //Escolhe uma das 4 operacoes
      int conta = rand_r(&seed) % 5; //Escolhe uma das 5 contas
      double valor = (rand_r(&seed) % 500) + 1; //Escolhe um valor de 1 a 500

      switch (operacao){
         case 0: {
            deposito(conta, valor);
            break;
         }
         case 1: {
            saque(conta, valor);
            break;
         }
         case 2: {
            consulta_saldo(conta);
            break;
         }
         case 3: {
            int destino = rand_r(&seed) % 5;
            transferencia(conta, destino, valor);
            break;
         }
      }
   }
}

void* child(void * buf) { 
   unsigned long childID = (unsigned long) buf;
   Operacoes(childID);
   return NULL;
}

unsigned long processCommandLine(int argc, char** argv) {
   if (argc == 2) {
      return strtoul(argv[1], 0, 10);
   } else if (argc == 1) {
      return 1;
   } else {
      fprintf(stderr, "\nUsage: ./mutualExclusion [evenNumberOfThreads]\n");
      exit(1);
   }
}

int main(int argc, char** argv) {
  pthread_t * children;                   // dynamic array of child threads
  unsigned long id = 0;                   // loop control variable
  unsigned long numThreads = 0;           // desired # of threads
                                          // get desired # of threads
      numThreads = processCommandLine(argc, argv);
      init_contas();
                                          // allocate array of handles
  children = malloc( numThreads * sizeof(pthread_t) );
                                          // FORK:
   for (id = 1; id < numThreads; id++) {
     pthread_create( &(children[id-1]),   // our handle for the child 
                      NULL,               // attributes of the child
                      child,              // the function it should run
                      (void*) id );       // args to that function
   }

   Operacoes(0);

  for (id = 1; id < numThreads; id++) {   // JOIN:
      pthread_join( children[id-1], NULL );
   }

   printf("\nSaldos finais com %lu threads:\n", numThreads);

   for (int c = 0; c < 5; c++){
      printf("Conta %d: R$ %.2f\n", c, contas[c].saldo);
   }

  free(children);                         // deallocate array
  limpar();                               // deallocate mutex
   return 0;
}

