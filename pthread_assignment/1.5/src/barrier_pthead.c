#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../include/timer.h"

int thread_count;
pthread_barrier_t barrier;


void *func(void *args){
    (void)args;

    int N = 1000000;

    for(int i = 0 ; i < N ; i++)
        pthread_barrier_wait(&barrier);

    return NULL;
}



int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Incorrect input!");
        return -1;
    }

    // initialize the number and allocate space for the requested threads
    thread_count = strtol(argv[1],NULL,10);
    long thread;
    pthread_t* thread_handles = malloc(thread_count * (sizeof(pthread_t)));
    if(thread_handles == NULL)
        return -1;
        
    double start, finish;


    // initialize pthread_barrier
    if (pthread_barrier_init(&barrier, NULL,thread_count) != 0) {
        printf("pthread_barrier_init failed!\n");
        return -1;
    }

    GET_TIME(start);

    for(thread = 0 ; thread < thread_count ; thread++){
        if(pthread_create(&thread_handles[thread],NULL,func,NULL)!= 0){
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

    pthread_barrier_destroy(&barrier);
    free(thread_handles);

    
    return 0;
}
