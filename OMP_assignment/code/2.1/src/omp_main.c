

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "omp_multiplying_polynomials.h"
#include <stdbool.h>

double get_time_sec() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec*1e-9;
}


// USAGE:  ./test_main <Polynomial Degree> <number of threads>

int main( int argc, char *argv[]){

    // Το πρόγραμμά  λαμβάνει σαν ορίσματα τον βαθμό n του κάθε πολυωνύμου και τον αριθμό των νημάτων
    if (argc != 3) {
       
        return -1;
    }
    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    printf("Polynomial Degree: %d\n", n);
    printf("Number of Threads: %d\n", num_threads);
    

    // Δημιουργία 2 πλήρη τυχαία πολυωνύματων βαθμού n
    srand(time(NULL));

    // 1. Δημιουργία των πολυωνύμων εισόδου
    // Ένα πολυώνυμο n βαθμού έχει n+1 συντελεστές (από x^0 έως x^n)
    int size_n = n + 1;
    int size_r = 2 * n + 1; // Το αποτέλεσμα είναι βαθμού 2n
    double tt = get_time_sec();
    int *P1 = malloc(size_n * sizeof(int));
    if(P1 == NULL){
        printf("Failed memory allocation!");
        return -1;
    }
    int *P2 = malloc(size_n * sizeof(int));
    if(P2 == NULL){
        printf("Failed memory allocation!");
        return -1;
    }

    int *R_serial = calloc(size_r, sizeof(int)); 
    int *R_parallel = calloc(size_r, sizeof(int));

    // Δημιουργία τυχαίων μη-μηδενικών συντελεστών
    for (int i = 0; i < size_n; i++) {
        P1[i] = get_random_non_zero();
        P2[i] = get_random_non_zero();
    }
    double tt2 = get_time_sec();

    //1 --- Χρόνος Αρχικοποίησης ---
    printf("Initialization Time: %f seconds\n", tt2 - tt);
    
    // 2. Υπολογισμός του πολυωνύμου αποτελέσματος R = P1 * P2
    double t0 = get_time_sec();
    serial(P1 , P2, R_serial, n);
    double t1 = get_time_sec();

    // --- Χρόνος Παράλληλης Εκτέλεσης ---
    double t2 = get_time_sec();
    parallel(P1 , P2, R_parallel, n, num_threads);
    double t3 = get_time_sec();
    
    printf("Serial Time: %f seconds\n", t1 - t0);
    printf("Parallel Time: %f seconds\n", t3 - t2);
    
    if ((t3 - t2) > 0) {
        printf("Speedup: %f\n", (t1 - t0) / (t3 - t2));
        printf("Efficiency: %f\n", ((t1 - t0) / (t3 - t2)) / num_threads );
    }

    bool flag = true; 
    
    for (int i = 0; i < 2*n+1; i++) {
        if (R_serial[i] != R_parallel[i]) {
            flag = false ;
        }
       
    }
    if ( flag == true ){
        printf("Verification successful! Serial and Parallel results MATCH.\n");
    } else {
        printf("Verification FAILED! Serial and Parallel results DO NOT match.\n");
    }        
    
    free(P1);
    free(P2);

    free(R_serial);
    free(R_parallel);

    return 0;
}