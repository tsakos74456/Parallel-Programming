#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timer.h"
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>

typedef struct {
    int rows; // number of rows (matrix is square: rows = columns)
    int nnz; // number of non zero values
    int *row_idx;   // row index
    int *col_idx;   // column_index
    int *values;
} CSR;

// builds the square matrix with the according percentage of zero-values
void build_table(int **table, const int table_size, const int percent);

// builds the random vector
void build_vector(int *vector, const int table_size);

// MULTIPLY USING THE DENSE MATRIX
void dense_parallel_multiply(int *table, int *vector, int *result,int rows_per_proc, int N);

// MULTIPLY USING CSR
CSR serial_build_csr_table(int **table, const int N);
void csr_serial_multiply(CSR *csr, int *vector, int *result);


// print functions for debugging
void print_result_vector(const int *result, int size);
void print_table(int **table, int table_size);
void print_vector(int *vector, int table_size);

void swap_vectors(int **vec_in, int **vec_out);
bool sanity_check(const int *result1, const int *result2, int N);