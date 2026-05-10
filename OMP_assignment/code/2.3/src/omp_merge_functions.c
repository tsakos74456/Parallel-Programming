#include "merge_functions.h"

// Όριο (cutoff) για την αποφυγή δημιουργίας υπερβολικά πολλών tasks

int* creation_of_unsorted_array(int size_of_table_ints){

    srand(42); // Seed for reproducibility

    int *table = malloc(size_of_table_ints * sizeof(int));
    if (table == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for %d elements.\n", size_of_table_ints);
        exit(1); // Τερματισμός του προγράμματος με κωδικό σφάλματος
    }
    for ( int i = 0 ; i < size_of_table_ints ; i++){
        table[i] = rand() % 10000000;
    }
    return table;
}

void merge(int table[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Χρήση malloc αντί για stack arrays για αποφυγή SegFault
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    if (L == NULL || R == NULL) {
        perror("failed memory alocation in merge");
        exit(1);
    }

    for (int i = 0; i < n1; i++) {
        L[i] = table[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = table[m + 1 + j];
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            table[k] = L[i];
            i++;
            k++;
        }
        else {
            table[k] = R[j];
            k++;
            j++;
        }
    }

    while (i < n1) {
        table[k++] = L[i++];
    }
    while (j < n2) {
        table[k++] = R[j++];
    }

    free(L);
    free(R);
}


void serial_mergesort(int *table, int start, int finish) {
    if (start < finish) {
        int mid = start + (finish - start) / 2;
        serial_mergesort(table, start,mid );
        serial_mergesort(table, mid + 1, finish);
        merge(table, start, mid, finish);
    }
}

// Βοηθητική αναδρομική συνάρτηση για τα tasks
void parallel_recursive(int *table, int start, int finish) {
    if (start < finish) {
        int mid = start + (finish - start) / 2;

        // Χρήση του if() clause: Αν το υπο-πρόβλημα είναι μικρό, μην φτιάχνεις task
        #pragma omp task shared(table) if((finish - start) > TASK_THRESHOLD)
        parallel_recursive(table, start, mid);

        #pragma omp task shared(table) if((finish - start) > TASK_THRESHOLD)
        parallel_recursive(table, mid + 1, finish);

        #pragma omp taskwait
        merge(table, start, mid, finish);
    }
}

void parallel_mergesort(int *table, int size, int threads) {
    omp_set_num_threads(threads);
    #pragma omp parallel
    {
        #pragma omp single 
        parallel_recursive(table, 0, size - 1);
    }
}