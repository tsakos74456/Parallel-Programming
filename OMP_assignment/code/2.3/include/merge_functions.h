
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define TASK_THRESHOLD 10000
int* creation_of_unsorted_array(int size_of_table_ints);

void merge(int table[], int l, int m, int r);

void serial_mergesort(int *table, int start, int finish) ;
// Βοηθητική αναδρομική συνάρτηση για τα tasks
void parallel_recursive(int *table, int start, int finish) ;

void parallel_mergesort(int *table, int size, int threads) ;