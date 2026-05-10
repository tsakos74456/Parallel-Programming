#include "../include/functions_bank_threads.h"
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void *bank_coarse_mutex(void *args){
    long my_rank = (long)args;

    unsigned int seed = time(NULL) ^ (my_rank * 1234567);
    long long total = 0;

    for (int i = 0 ; i < info.transactions_number ; i++){

        size_t pos_sender = rand_r(&seed) % info.table_size;
        size_t pos_receiver = rand_r(&seed) % info.table_size;

        // choose a randome number as the rand_r is a discrete distribution the split between transactiona and read_queries will be coorect
        // in worst case scenario the deviation will be smaller than 1% 
        int r = rand_r(&seed) % 100;


        if(r < info.percent){ 
            size_t pos = rand_r(&seed) % info.table_size;

            pthread_mutex_lock(&(info.global_mutex));
            // usleep(100);

            total +=info.accounts[pos];
            pthread_mutex_unlock(&(info.global_mutex));
        }
        else{
            // skip
            if(pos_sender == pos_receiver)
                continue;
            
            pthread_mutex_lock(&(info.global_mutex));

            // if the sender has 0 money skip it and write sender has insufficient balance
            if (info.accounts[pos_sender] == 0){
                #ifdef DEBUG
                printf("Insufficient balance!\n");
                #endif
            
                pthread_mutex_unlock(&(info.global_mutex));
                continue;
            }
            long long amount = rand_r(&seed) % info.accounts[pos_sender];
            #ifdef DEBUG
            printf("Amount of trasaction:%lld.\nBalances before transactions sender: %lld, receiver: %lld\n",amount, info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif
            info.accounts[pos_sender] -= amount;
            info.accounts[pos_receiver] += amount;
            #ifdef DEBUG
            printf("Balances after transactions sender: %lld, receiver: %lld\n\n---------------------------\n", info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif
            pthread_mutex_unlock(&(info.global_mutex));
        }
    }

    #ifdef DEBUG
    printf("The money which are calculated by thread %ld , are: %lld\n",my_rank,total);
    #endif

    return NULL;

}


void *bank_coarse_rw(void *args){
    long my_rank = (long)args;

    unsigned int seed = time(NULL) ^ (my_rank * 1234567);
    long long total = 0;

    for (int i = 0 ; i < info.transactions_number ; i++){
        
        size_t pos_sender = rand_r(&seed) % info.table_size;
        size_t pos_receiver = rand_r(&seed) % info.table_size;

        // choose a randome number as the rand_r is a discrete distribution the split between transactiona and read_queries will be coorect
        // in worst case scenario the deviation will be smaller than 1% 
        int r = rand_r(&seed) % 100;


        if(r < info.percent){  
            size_t pos = rand_r(&seed) % info.table_size;

            pthread_rwlock_rdlock(&(info.global_rw));
            // usleep(100);

            total +=info.accounts[pos];
            pthread_rwlock_unlock(&(info.global_rw));
        }
        
        else{
            // skip
            if(pos_sender == pos_receiver)
                continue;

            pthread_rwlock_wrlock(&(info.global_rw));
            
            // if the sender has 0 money skip it and write sender has insufficient balance
            if (info.accounts[pos_sender] == 0){
                pthread_rwlock_unlock(&(info.global_rw));
                #ifdef DEBUG
                printf("Insufficient balance!\n");
                #endif

                continue;
            }
            long long amount = rand_r(&seed) % info.accounts[pos_sender];

            #ifdef DEBUG
            printf("Amount of trasaction:%lld.\nBalances before transactions sender: %lld, receiver: %lld\n",amount, info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif
            info.accounts[pos_sender] -= amount;
            info.accounts[pos_receiver] += amount;
            #ifdef DEBUG
            printf("Balances after transactions sender: %lld, receiver: %lld\n\n---------------------------\n", info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif

            pthread_rwlock_unlock(&(info.global_rw));
        }
    }
    #ifdef DEBUG
    printf("The money which are calculated by thread %ld , are: %lld\n",my_rank,total);
    #endif   
    return NULL;
}

void *bank_fine_mutex(void *args){
    long my_rank = (long)args;

    unsigned int seed = time(NULL) ^ (my_rank * 1234567);
    long long total = 0;

    for (int i = 0 ; i < info.transactions_number ; i++){

        size_t pos_sender = rand_r(&seed) % info.table_size;
        size_t pos_receiver = rand_r(&seed) % info.table_size;
        
        int r = rand_r(&seed) % 100;

        if(r < info.percent){  
            size_t pos = rand_r(&seed) % info.table_size; 

            pthread_mutex_lock(&(info.mutex[pos]));

            // usleep(100);
            total +=info.accounts[pos];
            pthread_mutex_unlock(&(info.mutex[pos]));
        }
        else{
            // skip same person
            if(pos_sender == pos_receiver)
                continue;
            

            // always start with locking the smallest one so we don't end up on deadlock as all the threads will lock the same order so if 2 threads
            // have same pos of sender and receiver won't create a deadlock
            size_t a = MIN(pos_sender,pos_receiver);
            size_t b = MAX(pos_sender,pos_receiver);
            pthread_mutex_lock(&(info.mutex[a]));
            pthread_mutex_lock(&(info.mutex[b]));

            // if the sender has 0 money skip it and write sender has insufficient balance
            if (info.accounts[pos_sender] == 0){
                pthread_mutex_unlock(&(info.mutex[b]));
                pthread_mutex_unlock(&(info.mutex[a]));
                printf("Insufficient balance!\n");
                continue;
            }

            long long amount = rand_r(&seed) % info.accounts[pos_sender];
            #ifdef DEBUG
            printf("Amount of trasaction:%lld.\nBalances before transactions sender: %lld, receiver: %lld\n",amount, info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif
            info.accounts[pos_sender] -= amount;
            info.accounts[pos_receiver] += amount;
            #ifdef DEBUG
            printf("Balances after transactions sender: %lld, receiver: %lld\n\n---------------------------\n", info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif

            pthread_mutex_unlock(&(info.mutex[b]));
            pthread_mutex_unlock(&(info.mutex[a]));
        }
    }

    #ifdef DEBUG
    printf("The money which are calculated by thread %ld , are: %lld\n",my_rank,total);
    #endif    
    return NULL;

}

void *bank_fine_rw(void *args){
        long my_rank = (long)args;

    unsigned int seed = time(NULL) ^ (my_rank * 1234567);
    long long total = 0;

    for (int i = 0 ; i < info.transactions_number ; i++){

        size_t pos_sender = rand_r(&seed) % info.table_size;
        size_t pos_receiver = rand_r(&seed) % info.table_size;
        
        int r = rand_r(&seed) % 100;

        if(r < info.percent){  
            size_t pos = rand_r(&seed) % info.table_size; 

            pthread_rwlock_rdlock(&(info.rw[pos]));
            // usleep(100);
            total +=info.accounts[pos];
            pthread_rwlock_unlock(&(info.rw[pos]));
        }
        else{

            // skip same person
            if(pos_sender == pos_receiver)
                continue;
            

            // always start with locking the smallest one so we don't end up on deadlock as all the threads will lock the same order so if 2 threads
            // have same pos of sender and receiver won't create a deadlock
            size_t a = MIN(pos_sender,pos_receiver);
            size_t b = MAX(pos_sender,pos_receiver);
            pthread_rwlock_wrlock(&(info.rw[a]));
            pthread_rwlock_wrlock(&(info.rw[b]));

            // if the sender has 0 money skip it and write sender has insufficient balance
            if (info.accounts[pos_sender] == 0){
                pthread_rwlock_unlock(&(info.rw[b]));
                pthread_rwlock_unlock(&(info.rw[a]));
                printf("Insufficient balance!\n");
                continue;
            }

            long long amount = rand_r(&seed) % info.accounts[pos_sender];
            #ifdef DEBUG
            printf("Amount of trasaction:%lld.\nBalances before transactions sender: %lld, receiver: %lld\n",amount, info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif
            info.accounts[pos_sender] -= amount;
            info.accounts[pos_receiver] += amount;
            #ifdef DEBUG
            printf("Balances after transactions sender: %lld, receiver: %lld\n\n---------------------------\n", info.accounts[pos_sender],info.accounts[pos_receiver]);
            #endif

            pthread_rwlock_unlock(&(info.rw[b]));
            pthread_rwlock_unlock(&(info.rw[a]));
        }
    }

    #ifdef DEBUG
    printf("The money which are calculated by thread %ld , are: %lld\n",my_rank,total);
    #endif    
    return NULL;

}