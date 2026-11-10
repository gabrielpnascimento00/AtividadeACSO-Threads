#include <pthread.h>
#include <stdio.h>
#include <unistd.h> // For sleep()

// Function to be executed by the detached thread
void *detached_thread_function(void *arg) {
    printf("Detached thread: Started.\n");
    sleep(2); // Simulate some work
    printf("Detached thread: Finished.\n");
    pthread_exit(NULL); // Terminate the thread
}

int main() {
    pthread_t thread_id;
    int ret;

    // Create the thread
    ret = pthread_create(&thread_id, NULL, detached_thread_function, NULL);
    if (ret != 0) {
        perror("pthread_create failed");
        return 1;
    }

    // Detach the thread
    ret = pthread_detach(thread_id);
    if (ret != 0) {
        perror("pthread_detach failed");
        return 1;
    }

    printf("Main thread: Detached the child thread.\n");

    // The main thread can continue its work without waiting for the detached thread
    sleep(3); // Give the detached thread time to finish
    printf("Main thread: Exiting.\n");

    return 0;
}
