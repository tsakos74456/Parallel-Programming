
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../include/timer.h"


int thread_count;
int number;

void* increase(void *args){
    (void)args; 
    
    for(int i = 0 ; i < 1000000 ; i++){
        __atomic_fetch_add(&number,1,__ATOMIC_RELAXED);
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

    free(thread_handles);
    printf("The number after 1000000 loops per thread is: %d \n", number );

    
    return 0;
}
