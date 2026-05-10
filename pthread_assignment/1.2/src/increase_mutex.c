
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../include/timer.h"

int thread_count;
int number;
pthread_mutex_t mutex;

void* increase(void *args){
    (void)args; 
    
    for(int i = 0 ; i < 1000000 ; i++){
        pthread_mutex_lock(&mutex);
        number++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
};
int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Incorrect input!");
        return -1;
    }

    // initialize the number and allocate space for the requested threads
    thread_count = strtol(argv[1],NULL,10);
    number = 0;
    long thread;
    pthread_t* thread_handles = malloc(thread_count * (sizeof(pthread_t)));
    if(thread_handles == NULL)
        return -1;

        
    double start, finish;

    // initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("pthread_mutex_init failed!\n");
        return -1;
    }

    GET_TIME(start);
    for(thread = 0 ; thread < thread_count ; thread++){
        if(pthread_create(&thread_handles[thread],NULL,increase,NULL)!= 0){
            printf("pthread_create failed!\n");
            return -1;
        } 
    }

    for(thread = 0 ; thread < thread_count ; thread++){
        if(pthread_join(thread_handles[thread],NULL) != 0){
            printf("pthread_join failed!\n");
            return -1;
        } 
    }

    GET_TIME(finish);
    printf("Elapsed time = %e seconds\n", finish - start);
    
    pthread_mutex_destroy(&mutex);
    free(thread_handles);
    printf("The number after 1000000 loops per thread is: %d \n", number );

    
    return 0;
}
