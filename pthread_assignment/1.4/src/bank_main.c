#include "../include/functions_bank_threads.h"


// Usage: ./bank <table_size> <transactions> <percent> <lock_type> <threads>
// in type lock 0 = LOCK_COARSE_MUTEX, 1 = LOCK_COARSE_RW, 2 = LOCK_FINE_MUTEX, 3 = LOCK_FINE_RW


bank_info info;


int main(int argc, char *argv[]){
    if(argc != 6){
        printf("Usage: ./bank <table_size> <transactions> <percent> <lock_type> <threads>!\n");
        return -1;
    }

    // read args
    info.table_size = atoi(argv[1]);
    info.transactions_number = atoi(argv[2]);

    info.percent = atoi(argv[3]);
    if(info.percent < 0 || info.percent > 100){
        printf("Give a valid percent between 0 to 100!\n");
        return -1;
    }

    info.type_lock = atoi(argv[4]);
    info.thread_count = atoi(argv[5]);


    // initialize the table with the info
    info.accounts = malloc(info.table_size * sizeof(long long));
    if(info.accounts == NULL)
        return -2;

    srand(time(NULL));
    long long start_total = 0;
    for (size_t i = 0 ; i < info.table_size ; i++){
        // plus one to make sure the amount of money in the acc is bigger than 0
        info.accounts[i] = (rand() % 100000) + 1;
        start_total +=info.accounts[i];
    }

    double start, finish;

                
    #ifdef DEBUG
    // print table after initialization
    printf("Accounts:\n");
    for (size_t i = 0; i < info.table_size; i++) {
        printf("%lld ", info.accounts[i]);
    }
    printf("\n");
    #endif

    long thread;
    pthread_t* thread_handles = malloc(info.thread_count * (sizeof(pthread_t)));
    if(thread_handles == NULL)
        return -1;

    // implementation for coarse grain with mutex
    if(info.type_lock == LOCK_COARSE_MUTEX){
        // initialiaze mutex
        if(pthread_mutex_init(&(info.global_mutex),NULL) != 0){
            printf("Failed mutex initialization\n");
            return -1;
        }

        GET_TIME(start);

        // create threads
        for(thread = 0 ; thread < info.thread_count ; thread++){
            if(pthread_create(&thread_handles[thread],NULL,bank_coarse_mutex,(void*)thread)!= 0){
                printf("pthread_create failed!\n");
                return -1;
            }
        }

        // join threads
        for (thread = 0; thread < info.thread_count; thread++) {
            if(pthread_join(thread_handles[thread],NULL) != 0){
                printf("pthread_join failed!\n");
                return -1;
            }   
        }

        GET_TIME(finish);
        
        pthread_mutex_destroy(&(info.global_mutex));
    }




    // implementation for coarse grain with rwlock
    else if(info.type_lock == LOCK_COARSE_RW){
        // initialiaze rwlock
        if(pthread_rwlock_init(&(info.global_rw),NULL) != 0){
            printf("Failed mutex initialization\n");
            return -1;
        }

        GET_TIME(start);

        // create threads
        for(thread = 0 ; thread < info.thread_count ; thread++){
            if(pthread_create(&thread_handles[thread],NULL,bank_coarse_rw,(void*)thread)!= 0){
                printf("pthread_create failed!\n");
                return -1;
            }
        }

        // join threads
        for (thread = 0; thread < info.thread_count; thread++) {
            if(pthread_join(thread_handles[thread],NULL) != 0){
                printf("pthread_join failed!\n");
                return -1;
            }   
        }

        GET_TIME(finish);

        pthread_rwlock_destroy(&(info.global_rw));
    }




    // implementation for fine grain with mutex
    else if(info.type_lock == LOCK_FINE_MUTEX){
        // initialize mutex for each position of the table accounts
        info.mutex = malloc(info.table_size * (sizeof(pthread_mutex_t)));
        if(info.mutex == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
        for(size_t i = 0 ; i < info.table_size ; i++){
            if(pthread_mutex_init(&(info.mutex[i]),NULL) != 0){
                printf("Failed mutex initialization\n");
                return -1;
            }
        }

        GET_TIME(start);

        // create_threads
        for(thread = 0 ; thread < info.thread_count ; thread++){
            if(pthread_create(&thread_handles[thread],NULL,bank_fine_mutex,(void*)thread)!= 0){
                printf("pthread_create failed!\n");
                return -1;
            }
        }

        // join threads
        for (thread = 0; thread < info.thread_count; thread++) {
            if(pthread_join(thread_handles[thread],NULL) != 0){
                printf("pthread_join failed!\n");
                return -1;
            }   
        }

        GET_TIME(finish);

        // destroy mutexes and free
        for(size_t i = 0 ; i < info.table_size ; i++)
            pthread_mutex_destroy(&(info.mutex[i]));
        
        free(info.mutex);
    }




    // implementation for fine grain with rwlock
    else if(info.type_lock == LOCK_FINE_RW){
        // initialize rwlock for each position of the table accounts
        info.rw = malloc(info.table_size * (sizeof(pthread_rwlock_t)));
        if(info.rw == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
        for(size_t i = 0 ; i < info.table_size ; i++){
            if(pthread_rwlock_init(&(info.rw[i]),NULL) != 0){
                printf("Failed mutex initialization\n");
                return -1;
            }
        }

        GET_TIME(start);

        // create_threads
        for(thread = 0 ; thread < info.thread_count ; thread++){
            if(pthread_create(&thread_handles[thread],NULL,bank_fine_rw,(void*)thread)!= 0){
                printf("pthread_create failed!\n");
                return -1;
            }
        }

        // join threads
        for (thread = 0; thread < info.thread_count; thread++) {
            if(pthread_join(thread_handles[thread],NULL) != 0){
                printf("pthread_join failed!\n");
                return -1;
            }   
        }

        // destroy mutexes and free
        for(size_t i = 0 ; i < info.table_size ; i++)
            pthread_rwlock_destroy(&(info.rw[i]));
        
        GET_TIME(finish);

        free(info.rw);
    }

    long long total_end = 0;
    for (size_t i = 0 ; i < info.table_size ; i++)
        total_end +=info.accounts[i];
    
    if(total_end == start_total)
        printf("ALL GOOD!\n");
    else
        printf("Something is incorrect");
        
    printf("Elapsed time = %e seconds\n", finish - start);

    free(info.accounts);
    free(thread_handles);
}

