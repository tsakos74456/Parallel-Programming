#include "../include/functions.h"

// Usage ./mul <number of rows and columns(square matix)> <percentage of zero-values> <number of repetition> <number of threads>



int main(int argc, char *argv[]){
    // for time
    double start, finish;

    if(argc != 5){
        printf("Incorrect args.The correct usage is: ./mul <number of rows and columns(square matix)> <percentage of zero-values> <number of repetition> <number of threads> \n");
        return -1;
    }

    int table_size = atoi(argv[1]);
    int percent = atoi(argv[2]);
    int num_rep = atoi(argv[3]);
    int thread_count = atoi(argv[4]);

    // with this way of declaring and allocating table. It is saved contiguously in memory.
    int **table = malloc(table_size * (sizeof(int*)));
    if(table == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    *table = malloc (table_size * table_size * sizeof(int));
    if(*table == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }
    
    for (int i = 1; i < table_size; i++)
        table[i] = table[0] + i * table_size;
    int *original_vector = malloc (table_size * (sizeof(int)));
    if(original_vector == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }
    srand(time(NULL));

    // build the table and the vector serially
    build_table(table,table_size,percent);
    build_vector(original_vector,table_size);

    printf("-----Serial-----\n");
    
    // build csr table serially
    GET_TIME(start);
    CSR csr_table = serial_build_csr_table(table,table_size);
    GET_TIME(finish);
    printf("Elapsed time for building csr table serially is: %e seconds\n", finish - start);
    
    
    int *result = malloc(csr_table.rows * sizeof(int));
    if(result == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }
    
    // allocating space to save the result-vector of each implementation
    int *final_serial_csr_vector = malloc(table_size * sizeof(int));
    if(final_serial_csr_vector == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    int *final_serial_dense_vector = malloc(table_size * sizeof(int));
    if(final_serial_dense_vector == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    int *final_parallel_csr_vector = malloc(table_size * sizeof(int));
    if(final_parallel_csr_vector == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    int *final_parallel_dense_vector = malloc(table_size * sizeof(int));
    if(final_parallel_dense_vector == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    // multiply serially using the dense (original) table
    int *vector_dense = malloc(table_size * (sizeof(int)));
    if(vector_dense == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    memcpy(vector_dense,original_vector,table_size * (sizeof(int)));

    GET_TIME(start);
    for(int i = 0 ; i < num_rep ; i++){
        dense_serial_multiply(table,vector_dense,result,table_size);
        if(i != num_rep -1)
            swap_vectors(&vector_dense, &result);
    }
    
    GET_TIME(finish);
    printf("Elapsed time for multiplying the the dense matrix with the vector serially is: %e seconds\n", finish - start);

    // for sanity check
    memcpy(final_serial_dense_vector,result,table_size * (sizeof(int)));

    // multiply serially using the csr table
    int *vector_csr = malloc(table_size * (sizeof(int)));
    if(vector_csr == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    memcpy(vector_csr,original_vector,table_size * (sizeof(int)));

    
    GET_TIME(start);
    for(int i = 0 ; i < num_rep ; i++){
        csr_serial_multiply(&csr_table,vector_csr,result);
        if(i != num_rep - 1)
            swap_vectors(&vector_csr, &result);
    }
    
    GET_TIME(finish);
    printf("Elapsed time for multiplying the csr matrix with vector serially is: %e seconds\n", finish - start);


    // for sanity check 
    memcpy(final_serial_csr_vector,result,table_size * (sizeof(int)));

    //        PARALLEL      //
    // -------------------- //
    // -------------------- //
    // -------------------- //

    printf("-----Parallel-----\n");

    // BUILD CSR TABLE parallel
    omp_set_num_threads(thread_count);

    GET_TIME(start);
    CSR csr_parallel = parallel_build_csr_table(table,table_size);
    GET_TIME(finish);
    
    printf("Elapsed time for building csr table parallelly is: %e seconds\n", finish - start);
    
    // multiply parallelly using the dense (original) table
    memcpy(vector_dense,original_vector,table_size * (sizeof(int)));

    GET_TIME(start);
    for(int i = 0 ; i < num_rep ; i++){
        dense_parallel_multiply(table,vector_dense,result,table_size);
        if(i != num_rep -1)
            swap_vectors(&vector_dense, &result);
    }
    GET_TIME(finish);
    
    printf("Elapsed time for multiplying the the dense matrix with the vector parallelly is: %e seconds\n", finish - start);
    memcpy(final_parallel_dense_vector,result,table_size * (sizeof(int)));


    // multiply parallelly using the csr table
    memcpy(vector_csr,original_vector,table_size * (sizeof(int)));

    GET_TIME(start);
    for(int i = 0 ; i < num_rep ; i++){
        csr_parallel_multiply(&csr_parallel,vector_csr,result);
        if(i != num_rep -1)
            swap_vectors(&vector_csr, &result);
    }
    GET_TIME(finish);
    
    
    printf("Elapsed time for multiplying the csr matrix with vector parallelly is: %e seconds\n", finish - start);
    memcpy(final_parallel_csr_vector,result,table_size * (sizeof(int)));

    if(sanity_check(final_serial_dense_vector,final_serial_csr_vector,final_parallel_dense_vector,final_parallel_csr_vector,table_size))
        printf("All the resuls are matching and correct. Good job\n");

    free(final_serial_dense_vector);
    free(final_serial_csr_vector);
    free(final_parallel_csr_vector);
    free(final_parallel_dense_vector);

    free(*table);
    free(table);
    free(original_vector);
    free(vector_csr);
    free(vector_dense);
    free(result);

    free(csr_table.row_idx);
    free(csr_table.col_idx);
    free(csr_table.values);
    free(csr_parallel.row_idx);
    free(csr_parallel.col_idx);
    free(csr_parallel.values);

    return 0;
}