#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

typedef struct {
  
    int *P1;         // Συντελεστές του 1ου πολυωνύμου
    int *P2;         // Συντελεστές του 2ου πολυωνύμου
    int *R;          // Συντελεστές του πολυωνύμου αποτελέσματος
    int n;           // Βαθμός των πολυωνύμων εισόδου
    int thread_id;   // ID του νήματος για κατανομή εργασίας
    int num_threads; // Συνολικός αριθμός νημάτων

} ThreadData;





void *thread_worker(void *arg);


int get_random_non_zero() ;

void serial(int *P1, int *P2, int *R, int n);

void parallel( int *P1, int *P2, int *R, int n, int num_threads);