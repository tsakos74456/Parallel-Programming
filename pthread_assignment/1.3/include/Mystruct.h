#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>



struct array_stats_s {
    long long int info_array_0;
    long long int info_array_1;
    long long int info_array_2;
    long long int info_array_3;
};

// Δήλωση της καθολικής μεταβλητής (πες στον compiler ότι υπάρχει κάπου αλλού)
extern struct array_stats_s array_stats;

struct thread_args {
    long long int *array;
    int id;                // 0,1,2,3 -> σε ποιο πεδίο γράφουμε
    int elements;
};




void initialize_arrays( long long int *array_0, long long int *array_1,  long long int *array_2, long long int *array_3, long long int num_of_elements);

void *count_nonzero(void *args);

void thread_processing( long long int *array_0, long long int *array_1,  long long int *array_2, long long int *array_3, long long int num_of_elements);


void analyze_arrays_serial(long long int *array_0,long long int *array_1,long long int *array_2,long long int *array_3,long long int num_of_elements);



struct thread_args2 {
    long long int *array_0;
    long long int *array_1;
    long long int *array_2;
    long long int *array_3;
    long long int start;
    long long int end;
};

void *count_nonzero_multi(void *arg) ;


