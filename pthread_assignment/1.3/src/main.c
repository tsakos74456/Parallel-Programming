#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "Mystruct.h"

double get_time_sec() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec*1e-9;
}


int main(int argc , char *argv[]){

    long long int num_of_elements =  atoll(argv[1]);
    // printf("Number of elements per array: %lld\n", num_of_elements);

    if (argc < 2){
        return 1;
    }   
    printf("Number of Threads (deafult): 4 \n");
    // printf("Starting...\n");
    double t0 = get_time_sec();
    //δεσμυεση  μνυμης γαι τους πινακες της δομης ολοι εχουν το ιδιο μεγεθος
    long long int *array_0 = (long long int *) malloc ( num_of_elements * sizeof(long long int) );
    long long int *array_1 = (long long int *) malloc ( num_of_elements * sizeof(long long int) );
    long long int *array_2 = (long long int *) malloc ( num_of_elements * sizeof(long long int) );
    long long int *array_3 = (long long int *) malloc ( num_of_elements * sizeof(long long int) );
    //αρχικοποίηση των πινάκων με τιμες απο 0 το 9
    initialize_arrays(array_0, array_1, array_2, array_3, num_of_elements);
    double t1 = get_time_sec();
    // printf("Arrays initialized in %f seconds.\n", t1 - t0);
    

    // ******* ΣΕΙΡΙΑΚΗ ΑΝΑΛΥΣΗ******* 
    double t2 = get_time_sec();
    analyze_arrays_serial(array_0, array_1, array_2, array_3, num_of_elements);
    double t3 = get_time_sec();
    

    // ******* ΑΠΟΘΗΚΕΥΣΗ ΣΕΙΡΙΑΚΩΝ ΑΠΟΤΕΛΕΣΜΑΤΩΝ ΓΙΑ ΕΛΕΓΧΟ *******
    struct array_stats_s serial_results = array_stats; 
    // Αυτή η τοπική μεταβλητή κρατά τα αποτελέσματα της σειριακής.



    //  *******ΠΑΡΑΛΛΗΛΗ ΑΝΑΛΥΣΗ  *******
    double t4 = get_time_sec();
    thread_processing(array_0, array_1, array_2, array_3, num_of_elements);
    double t5 = get_time_sec();
    




    printf("Initialization Time: %f seconds\n", t1 - t0);
    printf("Serial Time: %f seconds\n", t3 - t2);
    printf("Parallel Time: %f seconds\n", t5 - t4);

    //Επιτάχυνση  παράλληλου προγράμματος
    double speedup = (t3 - t2) / (t5 - t4);
    printf("Speedup (Serial / Parallel): %f\n", speedup);  
    printf("Efficiency: %.2f\n", ( ((t3 - t2) / (t5 - t4)) / 4 )  );

    
    //ελεγχος αν τα αποτελεσματα ειναι ιδια

    bool check_ok = true;
    if (serial_results.info_array_0 != array_stats.info_array_0) check_ok = false;
    if (serial_results.info_array_1 != array_stats.info_array_1) check_ok = false;
    if (serial_results.info_array_2 != array_stats.info_array_2) check_ok = false;
    if (serial_results.info_array_3 != array_stats.info_array_3) check_ok = false;

    if (check_ok) {
        printf("Verification successful! Serial and Parallel results MATCH.\n");
    } else {
        printf("Verification FAILED! Serial and Parallel results DO NOT match.\n");
    }
    


    // απελευθερωση μνημης
    free(array_0);
    free(array_1);
    free(array_2);
    free(array_3); 


    return 0;
}