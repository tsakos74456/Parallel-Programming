#define _POSIX_C_SOURCE 199309L

#include "merge_functions.h"

double get_time_sec() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec*1e-9;
}

bool is_sorted(int *table, int size) {
    for (int i = 0; i < size - 1; i++) {
        if (table[i] > table[i+1]) return 0;
    }
    return 1;
}

// Usage: ./test_main  <table size> <serial/parallel> <number of threads>


int main(int argc , char *argv[] ){
    if (argc != 4){
        printf("no correct inline parametres\n");
        return 1;
    }
    int size_of_table_ints = atoi(argv[1]);
    int number_of_threads = atoi(argv[3]);

    int* table = creation_of_unsorted_array(size_of_table_ints);

    if (strcmp(argv[2], "serial") == 0){
      
        //do serial processing
        double tt = get_time_sec();
        serial_mergesort(table , 0 ,size_of_table_ints - 1);
        double tt2 = get_time_sec();
        printf("Serial Time:%f\n",tt2 - tt );
        fflush(stdout);

    }
    else if (strcmp(argv[2], "parallel") == 0){
        //do parallel processing
        double tt = get_time_sec();
        parallel_mergesort(table , size_of_table_ints  , number_of_threads);
        double tt2 = get_time_sec();
        printf("Parallel Time:%f\n",tt2 - tt);
        fflush(stdout);
       
    }

    if (is_sorted(table, size_of_table_ints)) {
        printf("Sorted \n") ;
        // for(int  i = 0 ; i < size_of_table_ints -1 ; i++){
        //     printf("%d \n" ,table[i]);
        // }
               
    } 
    else 
        printf("Array not sorted!\n");
    
    free(table);
    
    return 0;
}