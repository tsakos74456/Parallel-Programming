#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../include/timer.h"

int thread_count;
pthread_mutex_t mutex;
pthread_cond_t cond;
int barrier_thread_count = 0;

void *func(void *args){
    (void)args;

    int N = 1000000;

    for(int i = 0 ; i < N ; i++){
        pthread_mutex_lock(&mutex);
        barrier_thread_count++;

        if(barrier_thread_count == thread_count){
            barrier_thread_count = 0 ;
            pthread_cond_broadcast(&cond);
        }
        else
            while(pthread_cond_wait(&cond,&mutex) != 0);
            
        pthread_mutex_unlock(&mutex);
    }

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


    // initialize mutex
    if (pthread_mutex_init(&mutex,NULL) != 0) {
        printf("pthread_barrier_init failed!\n");
        return -1;
    }

    // initialize condition variable
    if (pthread_cond_init(&cond,NULL) != 0) {
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

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    free(thread_handles);

    
    return 0;
}
