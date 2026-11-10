#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep

// Function to be executed by the new thread
void *thread_function(void *arg) {
    printf("Detached thread is running.\n");
    sleep(2); // Simulate some work
    printf("Detached thread is exiting.\n");
    pthread_exit(NULL); // Terminate the thread
}

int main() {
    pthread_attr_t attr; // Declare a thread attribute object
    pthread_t tid;       // Declare a thread ID variable
    int ret;             // For return values of pthreads functions

    // 1. Initialize the thread attribute object with default attributes
    ret = pthread_attr_init(&attr);
    if (ret != 0) {
        perror("pthread_attr_init failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set the detach state attribute to PTHREAD_CREATE_DETACHED
    ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (ret != 0) {
        perror("pthread_attr_setdetachstate failed");
        pthread_attr_destroy(&attr); // Clean up attribute object
        exit(EXIT_FAILURE);
    }

    // 3. Create the thread with the specified attributes
    ret = pthread_create(&tid, &attr, thread_function, NULL);
    if (ret != 0) {
        perror("pthread_create failed");
        pthread_attr_destroy(&attr); // Clean up attribute object
        exit(EXIT_FAILURE);
    }

    // 4. Destroy the thread attribute object when done with it
    ret = pthread_attr_destroy(&attr);
    if (ret != 0) {
        perror("pthread_attr_destroy failed");
        exit(EXIT_FAILURE);
    }

    printf("Main thread created a detached thread and is now exiting.\n");
    // Main thread exits, but the detached thread might continue running for a short time
    // before the process terminates (if no other joinable threads exist).
    pthread_exit(NULL); // Terminate the main thread
}
