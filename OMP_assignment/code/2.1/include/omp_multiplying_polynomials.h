#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>










int get_random_non_zero() ;

void serial(int *P1, int *P2, int *R, int n);

void parallel( int *P1, int *P2, int *R, int n, int num_threads);