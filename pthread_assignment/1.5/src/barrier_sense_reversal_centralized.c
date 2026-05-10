#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../include/timer.h"
#include <stdbool.h>


typedef struct _thread_barrier{
    int barrier_thread_count;       //cuurent amount of threads which have reached into barrier
    int thread_count;               // max number of threads
    pthread_mutex_t mutex;
    volatile bool flag;
} thread_barrier;

thread_barrier barrier;

void barrier_wait(thread_barrier *barrier,bool *local_sense){
    // give the opposite value
    *local_sense = !*(local_sense);

    pthread_mutex_lock(&(barrier->mutex));
    barrier->barrier_thread_count += 1;
        
    // check if this is the last thread and initialize the barrier so it can be used again
    if (barrier->barrier_thread_count == barrier->thread_count){
        barrier->barrier_thread_count = 0;
        barrier->flag = *local_sense;
        pthread_mutex_unlock(&(barrier->mutex));
    } 
    else {
        pthread_mutex_unlock(&(barrier->mutex));
        while (barrier->flag != *local_sense); // wait for flag
    }
}


void *func(void *args){
    (void)args;

    int N = 100;
    bool local_sense = false;
    for(int i = 0 ; i < N ; i++){
        barrier_wait(&barrier,&local_sense);
    }

    return NULL;
}



int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Incorrect input!");
        return -1;
    }

    // initialize the number and allocate space for the requested threads
    barrier.thread_count = strtol(argv[1],NULL,10);
    long thread;
    pthread_t* thread_handles = malloc(barrier.thread_count * (sizeof(pthread_t)));
    if(thread_handles == NULL)
        return -1;
    
    double start, finish;


    // initialize the thread_barrier
    if (pthread_mutex_init(&(barrier.mutex),NULL) != 0) {
        printf("pthread_barrier_init failed!\n");
        return -1;
    }
    barrier.flag = 0;
    barrier.barrier_thread_count = 0;

    GET_TIME(start);

    for(thread = 0 ; thread < barrier.thread_count ; thread++){
        if(pthread_create(&thread_handles[thread],NULL,func,NULL)!= 0){
            printf("pthread_create failed!\n");
            return -1;
        } 
    }

    for(thread = 0 ; thread < barrier.thread_count ; thread++){
        if(pthread_join(thread_handles[thread],NULL) != 0){
            printf("pthread_join failed!\n");
            return -1;
        } 
    }

    GET_TIME(finish);
    printf("Elapsed time = %e seconds\n", finish - start);

    pthread_mutex_destroy(&(barrier.mutex));
    free(thread_handles);

    
    return 0;
}