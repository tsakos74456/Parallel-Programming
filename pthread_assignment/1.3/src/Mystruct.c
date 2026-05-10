#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <Mystruct.h>

struct array_stats_s array_stats;

// Ορισμός και αρχικοποίηση (μία φορά!)

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void initialize_arrays( long long int *array_0, long long int *array_1,  long long int *array_2, long long int *array_3, long long int num_of_elements){
    srand(time(NULL));   // αρχικοποιεί το σύστημα τυχαιότητας
    
    for (long long int i = 0 ; i < num_of_elements; i++){
        array_0[i] = rand() % 10; 
        array_1[i] = rand() % 10;
        array_2[i] = rand() % 10;
        array_3[i] = rand() % 10;

    }
    
}


void *count_nonzero(void *args){
    struct thread_args *data = (struct thread_args *) args;
    long long int count = 0;

    for (long long int i = 0; i < data->elements; i++){
        if (data->array[i] != 0){
            count++;
        }
    }

    switch (data->id){
        case 0:
            array_stats.info_array_0 = count;
            break;
        case 1:
            array_stats.info_array_1 = count;
            break;
        case 2:
            array_stats.info_array_2 = count;
            break;
        case 3:
            array_stats.info_array_3 = count;
            break;
        default:
            break;
    }

    pthread_exit(NULL);

}

void thread_processing( long long int *array_0, long long int *array_1,  long long int *array_2, long long int *array_3, long long int num_of_elements){
    //παραδειγμα επεξεργασιας των πινακων
    //ΤΙΜΕΣ ΠΡΙΝ ΑΥΞΗΘΟΥΝ ΚΑΤΑ 1
   

   

    pthread_t threadS[4];

    struct thread_args args[4];
    // αρχικοποίηση των δομών ορισμάτων για κάθε νήμα
    args[0].array = array_0;
    args[1].array = array_1;
    args[2].array = array_2;
    args[3].array = array_3;
   
    for (long long int i = 0; i < 4; i++){
        args[i].id = i;
        args[i].elements = num_of_elements;
        pthread_create(&threadS[i], NULL, count_nonzero, &args[i]);
    }

    for (int i = 0; i < 4; i++){
        pthread_join(threadS[i], NULL);
    }
    // // Εμφάνιση αποτελεσμάτων
    // printf("Parallel Analysis Results:\n");
    // printf("Array 0 - Non-zero elements: %lld\n", array_stats.info_array_0);
    // printf("Array 1 - Non-zero elements: %lld\n", array_stats.info_array_1);
    // printf("Array 2 - Non-zero elements: %lld\n", array_stats.info_array_2);
    // printf("Array 3 - Non-zero elements: %lld\n", array_stats.info_array_3);
   
}


void analyze_arrays_serial(long long int *array_0,long long int *array_1,long long int *array_2,long long int *array_3,long long int num_of_elements)
{
    array_stats.info_array_0 = 0;
    array_stats.info_array_1 = 0;
    array_stats.info_array_2 = 0;
    array_stats.info_array_3 = 0;

    for (long long int i = 0; i < num_of_elements; i++) {

        if (array_0[i] != 0)
            array_stats.info_array_0++;

        if (array_1[i] != 0)
            array_stats.info_array_1++;

        if (array_2[i] != 0)
            array_stats.info_array_2++;

        if (array_3[i] != 0)
            array_stats.info_array_3++;
    }

    // printf("Serial Analysis Results:\n");
    // printf("Array 0 - Non-zero elements: %lld\n", array_stats.info_array_0);
    // printf("Array 1 - Non-zero elements: %lld\n", array_stats.info_array_1);
    // printf("Array 2 - Non-zero elements: %lld\n", array_stats.info_array_2);
    // printf("Array 3 - Non-zero elements: %lld\n", array_stats.info_array_3);
}






void *count_nonzero_multi(void *arg) {
    struct thread_args2 *data = (struct thread_args2 *) arg;

    long long int c0 = 0, c1 = 0, c2 = 0, c3 = 0;

    for (long long int i = data->start; i < data->end; i++) {
        if (data->array_0[i] != 0){ 
            c0++;
        }
        if (data->array_1[i] != 0) {
            c1++;
        }
        if (data->array_2[i] != 0) {
            c2++;
        }
        if (data->array_3[i] != 0) {
            c3++;
        }
    }

    pthread_mutex_lock(&mtx);
    array_stats.info_array_0 += c0;
    array_stats.info_array_1 += c1;
    array_stats.info_array_2 += c2;
    array_stats.info_array_3 += c3;
    pthread_mutex_unlock(&mtx);

    return NULL;
}