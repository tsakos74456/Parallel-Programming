#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../include/timer.h"
#include <unistd.h>

// in type lock 0 = LOCK_COARSE_MUTEX, 1 = LOCK_COARSE_RW, 2 = LOCK_FINE_MUTEX, 3 = LOCK_FINE_RW
typedef enum{
    LOCK_COARSE_MUTEX,
    LOCK_COARSE_RW,
    LOCK_FINE_MUTEX,
    LOCK_FINE_RW 
}type_lock_t;

typedef struct info{
    long long *accounts;
    size_t table_size;
    type_lock_t type_lock;
    int transactions_number;
    int percent;
    int thread_count;

    pthread_mutex_t global_mutex; // for coarse grain with mutex
    pthread_rwlock_t global_rw; //for coarse grain with rwlocks

    pthread_mutex_t *mutex;     //for fine grain with mutex
    pthread_rwlock_t *rw; //for fine grain with rwlocks 

}bank_info;

extern bank_info info;

void *bank_coarse_mutex(void *args);

void *bank_coarse_rw(void *args);

void *bank_fine_mutex(void *args);

void *bank_fine_rw(void *args);