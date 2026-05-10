#include "../include/functions.h"

void build_table(int **table, const int table_size, const int percent){
    for (int i = 0 ; i < table_size ; i++) {
        for (int j = 0 ; j < table_size ; j++) {
            int r = rand() % 100;
            if (r < percent)
                table[i][j] = 0;
            else
                table[i][j] = (rand() % 99) + 1;
        }
    }
};

// zero values are not used in vector but only in the table it will have values from 0 ... 1000
void build_vector(int *vector, const int table_size){
    for(int i = 0 ; i < table_size ; i++){
        vector[i] = (rand() % 100) + 1;
    }
};

CSR serial_build_csr_table(int **table, const int N){
    CSR csr;
    csr.rows = N;
    csr.nnz = 0;

    // count non-zero values per row 
    int *nnz_per_row = malloc(N * sizeof(int));
    if(nnz_per_row == NULL){
        printf("Failed memory allocation!\n");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0 ; i < N ; i++){
        nnz_per_row[i] = 0;
        for(int j = 0 ; j < N ; j++)
            if(table[i][j] != 0){
                nnz_per_row[i]++;
                csr.nnz++;
            }
    }

    // create row_index
    csr.row_idx = malloc((N +1)* sizeof(int));
    if(csr.row_idx == NULL){
        printf("Failed memory allocation\n");
        exit(EXIT_FAILURE);
    }
    csr.row_idx[0] = 0;
    for(int i = 0 ; i < N ; i++)
        csr.row_idx[i + 1] = csr.row_idx[i] + nnz_per_row[i];

    csr.values = malloc(csr.nnz * sizeof(int));
    if(csr.values == NULL){
        printf("Failed memory allocation\n");
        exit(EXIT_FAILURE);
    }

    csr.col_idx = malloc(csr.nnz * sizeof(int));
    if(csr.col_idx == NULL){
        printf("Failed memory allocation\n");
        exit(EXIT_FAILURE);
    }

    // create column index and values
    for(int i = 0 ; i < N ; i++){
        int pos = csr.row_idx[i];
        for(int j = 0 ; j < N ; j++){
            
            if(table[i][j] != 0){
                csr.values[pos] = table[i][j];
                csr.col_idx[pos] = j;
                pos++;
            }
        }
    }

    free(nnz_per_row);
    return csr;
}

void csr_serial_multiply(CSR *csr, int *vector, int *result){
    for(int i = 0 ; i < csr->rows ; i++){
        int sum = 0;
        for( int j = csr->row_idx[i] ; j <csr->row_idx[i + 1] ; j++){
            sum += csr->values[j] * vector[csr->col_idx[j]];
        }
        result[i] = sum;
    }
}

void dense_serial_multiply(int **table, int *vector, int *result, int N){
    for(int i = 0 ; i < N ; i++){
        int sum = 0;
        for(int j = 0 ; j < N ; j++){
            sum += table[i][j] * vector[j];
        }
        result[i] = sum;
    }
}



void print_table(int **table, int table_size){
    for (int i = 0; i < table_size; i++) {
        for (int j = 0; j < table_size; j++) {
            printf("%4d ", table[i][j]);
        }
        printf("\n");
    }
};

void print_vector(int *vector, int table_size){
    for (int i = 0; i < table_size; i++) {
        printf("%4d \n", vector[i]);
    }
    printf("\n");
};

void print_csr(const CSR *csr){
    printf("\n--- CSR representation ---\n");

    printf("rows = %d\n", csr->rows);
    printf("nnz  = %d\n\n", csr->nnz);

    printf("row_idx:\n");
    for (int i = 0; i <= csr->rows; i++) {
        printf("%d ", csr->row_idx[i]);
    }
    printf("\n\n");

    printf("col_idx:\n");
    for (int i = 0; i < csr->nnz; i++) {
        printf("%d ", csr->col_idx[i]);
    }
    printf("\n\n");

    printf("values:\n");
    for (int i = 0; i < csr->nnz; i++) {
        printf("%d ", csr->values[i]);
    }
    printf("\n");
}

void print_result_vector(const int *result, int size){
    printf("\nResult vector:\n");
    for (int i = 0; i < size; i++) {
        printf("%d\n", result[i]);
    }
    printf("\n");
}

void swap_vectors(int **vec_in, int **vec_out){
    int *tmp = *vec_in;
    *vec_in = *vec_out;
    *vec_out = tmp;
}


//   PARALLEL IMPLEMENTATION
// ----------------------------
// ----------------------------

CSR parallel_build_csr_table(int **table, const int N){
    CSR csr;
    csr.rows = N;
    csr.nnz = 0;

    // count non-zero values per row 
    int *nnz_per_row = malloc(N * sizeof(int));
    if(nnz_per_row == NULL){
        printf("Failed memory allocation!\n");
        exit(EXIT_FAILURE);
    }
    
    #pragma omp parallel for \
        default(none) shared(table, nnz_per_row, N) schedule(dynamic,64)
    for(int i = 0 ; i < N ; i++){
        int count = 0;
        for(int j = 0 ; j < N ; j++){
            if(table[i][j] != 0)
                count++;
        }
        nnz_per_row[i] = count;
    }

    // create row_index
    csr.row_idx = malloc((N +1)* sizeof(int));
    if(csr.row_idx == NULL){
        printf("Failed memory allocation\n");
        exit(EXIT_FAILURE);
    }
    csr.row_idx[0] = 0;
    for(int i = 0 ; i < N ; i++)
        csr.row_idx[i + 1] = csr.row_idx[i] + nnz_per_row[i];

    csr.nnz = csr.row_idx[N];
    csr.values = malloc(csr.nnz * sizeof(int));
    if(csr.values == NULL){
        printf("Failed memory allocation\n");
        exit(EXIT_FAILURE);
    }

    csr.col_idx = malloc(csr.nnz * sizeof(int));
    if(csr.col_idx == NULL){
        printf("Failed memory allocation\n");
        exit(EXIT_FAILURE);
    }

    // create column index and values
    #pragma omp parallel for \
        default(none) shared(table,csr,N) schedule(dynamic,64)

    for(int i = 0 ; i < N ; i++){
        int pos = csr.row_idx[i];
        for(int j = 0 ; j < N ; j++){
            
            if(table[i][j] != 0){
                csr.values[pos] = table[i][j];
                csr.col_idx[pos] = j;
                pos++;
            }
        }
    }

    free(nnz_per_row);
    return csr;
}


void dense_parallel_multiply(int **table, int *vector, int *result, int N){
    #pragma omp parallel for \
        default(none) shared(table,vector,result,N) schedule(dynamic,64)
    for(int i = 0 ; i < N ; i++){
        int sum = 0;
        for(int j = 0 ; j < N ; j++){
            sum += table[i][j] * vector[j];
        }
        result[i] = sum;
    }
}
void csr_parallel_multiply(CSR *csr, int *vector, int *result){
    #pragma omp parallel for \
        default(none) shared(csr,vector,result) schedule(dynamic,64)
    for(int i = 0 ; i < csr->rows ; i++){
        int sum = 0;
        for( int j = csr->row_idx[i] ; j <csr->row_idx[i + 1] ; j++){
            sum += csr->values[j] * vector[csr->col_idx[j]];
        }
        result[i] = sum;
    }
}

bool sanity_check(const int *serial_dense, const int *serial_csr, const int *parallel_dense, const int *parallel_csr, int N){
    for(int i = 0; i < N; i++){
        if(serial_dense[i] != serial_csr[i]){
            printf("Mismatch between serial multiplication at index %d: %d != %d\n", i, serial_dense[i], serial_csr[i]);
            return 0;   
        }
        if(serial_csr[i] != parallel_csr[i]){
            printf("Mismatch between csr multiplication at index %d: %d != %d\n", i, serial_csr[i], parallel_csr[i]);
            return 0;  
        }
        if(parallel_dense[i] != parallel_csr[i]){
            printf("Mismatch between parallel multiplication at index %d: %d != %d\n", i, parallel_dense[i], parallel_csr[i]);
            return 0;  
        }
    }
    return 1;          
}