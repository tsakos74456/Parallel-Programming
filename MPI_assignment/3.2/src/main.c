#include "../include/functions.h"

// Usage ./matrx_mul <number of rows and columns(square matix)> <percentage of zero-values> <number of repetition> 



int main(int argc, char *argv[]){
    // for time
    
    int comm_sz, my_rank;

    // set mpi
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    
    int table_size, num_rep;
    int **table = NULL;
    int *original_vector = NULL;
    CSR csr_table;
    int *serial_dense_result = NULL;
    float time_for_building_csr_table = 0;
    // process 0 builds the table and the vector
    if(my_rank == 0){
        if (argc != 4) {
            printf("Usage: ./matrx_mul <size> <sparsity> <reps>\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        table_size = atoi(argv[1]);
        int percent = atoi(argv [2]);
        num_rep = atoi(argv[3]);

        // with this way of declaring and allocating table. It is saved contiguously in memory.
        table = malloc(table_size * (sizeof(int*)));
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
        original_vector = malloc (table_size * (sizeof(int)));
        if(original_vector == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
        srand(time(NULL));

        // build the table and the vector serially
        build_table(table,table_size,percent);
        build_vector(original_vector,table_size);


        printf("---------SERIAL------------\n");
        // build csr table as well
        double start, finish;
        GET_TIME(start);
        csr_table = serial_build_csr_table(table,table_size);
        GET_TIME(finish);

        time_for_building_csr_table = finish - start;
        printf("Building the CSR representation took: %f sec\n",time_for_building_csr_table);


        // serial multiply using dense representation
        serial_dense_result = malloc (table_size * (sizeof(int)));
        if(serial_dense_result == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
        
        int *vector_dense = malloc (table_size * (sizeof(int)));
        if(vector_dense == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
        memcpy(vector_dense,original_vector,table_size * (sizeof(int)));

        GET_TIME(start);
        for(int i = 0 ; i < num_rep ; i++){
            dense_parallel_multiply(*table,vector_dense,serial_dense_result,table_size,table_size);
            if(i != num_rep -1)
                swap_vectors(&vector_dense,&serial_dense_result);
        }
        GET_TIME(finish);

        printf("SERIAL-DENSE: %f sec\n",finish-start);


        // serial multiply using CSR representation
        int *serial_csr_result = malloc(table_size * (sizeof(int)));
        if(serial_csr_result == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }

        int *vector_csr = malloc(table_size * (sizeof(int)));
        if(vector_csr == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
        memcpy(vector_csr,original_vector,table_size * (sizeof(int)));

        GET_TIME(start);
        for(int i = 0 ; i < num_rep ; i++){
            csr_serial_multiply(&csr_table,vector_csr,serial_csr_result);
            if(i != num_rep -1)
                swap_vectors(&vector_csr,&serial_csr_result);
        }
        GET_TIME(finish);


        printf("SERIAL-CSR(+construction of csr represantation): %f sec\n",finish-start + time_for_building_csr_table);

        if(sanity_check(serial_csr_result,serial_dense_result,table_size))
            printf("Results with serial implemenation are matching and correct. Good job\n");
        else 
            printf("PROBLEM IN THE SANITY CHECK of serial implementation\n");

        free(vector_csr);
        free(serial_csr_result);
        free(vector_dense);
    }
    double start,finish;
    MPI_Barrier(MPI_COMM_WORLD);  
    start = MPI_Wtime();

    // send args to other processes
    MPI_Bcast(&table_size,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&num_rep,1,MPI_INT,0,MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);  
    finish = MPI_Wtime();

    float comm_time1 = finish - start;

    if(my_rank != 0){
        original_vector = malloc (table_size * (sizeof(int)));
        if(original_vector == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);  
    start = MPI_Wtime();

    // send the vector to other processes
    MPI_Bcast(original_vector,table_size, MPI_INT, 0, MPI_COMM_WORLD);
        
    MPI_Barrier(MPI_COMM_WORLD);  
    finish = MPI_Wtime();

    float comm_time2 = finish - start;

    /* -----------------------------------------------------------------------------
     -------------------------------------------------------------------------------
      THIS IS ABOUT THE MULTIPLICATION USING THE CSR REPRESENTATION OF THE MATRIX
     -------------------------------------------------------------------------------
     -------------------------------------------------------------------------------
    */
    if(my_rank == 0)
        printf("-------PARALLEL--------------\n");

    // the data will be sent with scatterv so process 0 has to calculate what each rank is going to calc
    CSR local_csr_table;
    int *p_vector_csr = malloc(table_size * (sizeof(int)));
    if(p_vector_csr == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }
    memcpy(p_vector_csr,original_vector,table_size * (sizeof(int)));


    int rows_per_proc = table_size / comm_sz;
    int remainder = table_size % comm_sz;
    int *local_rows = malloc(comm_sz * sizeof(int));
    if(local_rows == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    int *row_start = malloc(comm_sz * sizeof(int));
    if(row_start == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    int *row_end = malloc(comm_sz * sizeof(int));
    if(row_end == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    // split rowidx per process and send it
    for(int p = 0 ; p < comm_sz ; p++){
        local_rows[p] = rows_per_proc + (p < remainder ? 1 : 0);
        row_start[p] = p * rows_per_proc + (p < remainder ? p : remainder);  // +  min(r, rem).
        row_end[p] = row_start[p] + local_rows[p];
    }

    int *rowidx_counts = NULL;
    int *rowidx_displays = NULL;

    if (my_rank == 0) {
        rowidx_counts = malloc(comm_sz * sizeof(int));
        if(rowidx_counts == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }
        rowidx_displays = malloc(comm_sz * sizeof(int));
        if(rowidx_displays == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }

        for (int p = 0; p < comm_sz; p++) {
            rowidx_counts[p] = local_rows[p] + 1;
            rowidx_displays[p] = row_start[p];
        }
    }

    local_csr_table.rows = local_rows[my_rank];
    local_csr_table.row_idx = malloc((local_csr_table.rows + 1) * sizeof(int));
    if(local_csr_table.row_idx == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    MPI_Barrier(MPI_COMM_WORLD);  
    start = MPI_Wtime();

    if(my_rank == 0)
        MPI_Scatterv(csr_table.row_idx,rowidx_counts,rowidx_displays, MPI_INT, local_csr_table.row_idx,local_csr_table.rows + 1, MPI_INT, 0, MPI_COMM_WORLD);
    else
        MPI_Scatterv(NULL,rowidx_counts,rowidx_displays, MPI_INT, local_csr_table.row_idx,local_csr_table.rows + 1, MPI_INT, 0, MPI_COMM_WORLD);

    
    MPI_Barrier(MPI_COMM_WORLD);  
    finish = MPI_Wtime();

    float comm_time3 = finish - start;


    // normalization so it can find the correct nnz
    int base = local_csr_table.row_idx[0];
    for (int i = 0; i < local_csr_table.rows + 1; i++)
        local_csr_table.row_idx[i] -= base;

    
    // send number of non zero values per proc and col_idx
    int *nnz_counts = NULL;
    int *nnz_displays = NULL;

    if (my_rank == 0) {
        nnz_counts = malloc(comm_sz * sizeof(int));
        if(nnz_counts == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }

        nnz_displays = malloc(comm_sz * sizeof(int));
        if(nnz_displays == NULL){
            printf("Failed memory allocation!\n");
            return -1;
        }

        for (int r = 0; r < comm_sz; r++) {
            nnz_counts[r] = csr_table.row_idx[row_end[r]] - csr_table.row_idx[row_start[r]];
            nnz_displays[r] = csr_table.row_idx[row_start[r]];
        }
    }
    
    local_csr_table.nnz = local_csr_table.row_idx[local_csr_table.rows];

    local_csr_table.col_idx = malloc(local_csr_table.nnz * sizeof(int));
    if(local_csr_table.col_idx == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    local_csr_table.values  = malloc(local_csr_table.nnz * sizeof(int));
    if(local_csr_table.values == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    MPI_Barrier(MPI_COMM_WORLD);  
    start = MPI_Wtime();

    // send values
    if(my_rank == 0)
        MPI_Scatterv(csr_table.values,nnz_counts,nnz_displays, MPI_INT, local_csr_table.values, local_csr_table.nnz , MPI_INT, 0, MPI_COMM_WORLD);
    else
        MPI_Scatterv(NULL,nnz_counts,nnz_displays, MPI_INT, local_csr_table.values, local_csr_table.nnz , MPI_INT, 0, MPI_COMM_WORLD);

    // send column index 
    if(my_rank == 0)
        MPI_Scatterv(csr_table.col_idx, nnz_counts, nnz_displays, MPI_INT, local_csr_table.col_idx, local_csr_table.nnz, MPI_INT, 0, MPI_COMM_WORLD);
    else
        MPI_Scatterv(NULL, nnz_counts, nnz_displays, MPI_INT, local_csr_table.col_idx, local_csr_table.nnz, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);  
    finish = MPI_Wtime();

    float comm_time4 = finish - start;

    int *local_p_csr_result = malloc(local_csr_table.rows * sizeof(int));
    if(local_p_csr_result == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }    
    int *p_csr_whole_result = malloc(table_size * sizeof(int));
    if(p_csr_whole_result == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    } 

    MPI_Barrier(MPI_COMM_WORLD);  
    start = MPI_Wtime();

    // multiply 
    for(int i = 0 ; i < num_rep ;i++){
        if(local_csr_table.rows > 0)
            csr_serial_multiply(&local_csr_table,p_vector_csr,local_p_csr_result);
        MPI_Allgatherv(local_p_csr_result,local_rows[my_rank],MPI_INT,p_csr_whole_result,local_rows,row_start,MPI_INT,MPI_COMM_WORLD);
        if(i != num_rep -1)
            swap_vectors(&p_vector_csr,&p_csr_whole_result);
    }

    MPI_Barrier(MPI_COMM_WORLD);  
    finish = MPI_Wtime();

    float csr_time_par = finish - start;
    if(my_rank == 0){
        printf("ONLY the communication for CSR took: %f sec\n",comm_time1 + comm_time2 +comm_time3 + comm_time4);
        printf("ONLY the multiplication using CSR took: %f sec \n",csr_time_par);
        printf("Parallel-CSR(CSR construction + communication + multiplication) took: %f sec\n",comm_time1 + comm_time2 +comm_time3 + comm_time4 + csr_time_par + time_for_building_csr_table);

    }

    if(my_rank == 0){
        if(sanity_check(p_csr_whole_result,serial_dense_result,table_size))
            printf("Results of csr-parallel and serial-dense are matching and correct. Good job\n");
        else 
            printf("PROBLEM IN THE SANITY CHECK\n");

        free(rowidx_counts);
        free(rowidx_displays);
        free(nnz_counts);
        free(nnz_displays);
    }

    free(p_csr_whole_result);
    free(local_p_csr_result);

    free(local_csr_table.col_idx);
    free(local_csr_table.row_idx);
    free(local_csr_table.values);
    free(p_vector_csr);
    free(local_rows);
    free(row_start);
    free(row_end);

    /* -----------------------------------------------------------------------------
     -------------------------------------------------------------------------------
      THIS IS ABOUT THE MULTIPLICATION USING THE DENSE REPRESENTATION OF THE MATRIX
     -------------------------------------------------------------------------------
     -------------------------------------------------------------------------------
    */

    // now split the matrix and send it to processes
    int num_per_proc = table_size / comm_sz;
    remainder = table_size % comm_sz;

    int *local_table = malloc(num_per_proc * table_size*  sizeof(int));
    if(local_table == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }
    int *local_result = malloc(num_per_proc * sizeof(int));
    if(local_result == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    int *new_result_dense_MPI = malloc(table_size * sizeof(int));
    if(new_result_dense_MPI == NULL){
        printf("Failed memory allocation!\n");
        return -1;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    // scatter the matrix
    if(my_rank == 0)
        MPI_Scatter(table[0],num_per_proc * table_size ,MPI_INT, local_table, num_per_proc * table_size, MPI_INT, 0, MPI_COMM_WORLD);
    else 
        MPI_Scatter(NULL,num_per_proc * table_size, MPI_INT, local_table, num_per_proc * table_size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    finish = MPI_Wtime();

    comm_time1 = finish - start;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    for(int i = 0 ; i < num_rep ; i++){
        dense_parallel_multiply(local_table,original_vector,local_result,num_per_proc,table_size);

        MPI_Allgather(local_result, num_per_proc , MPI_INT, new_result_dense_MPI, num_per_proc , MPI_INT, MPI_COMM_WORLD);

        // in case of there are some remained rows, process 0 will calculate them 
        if(remainder > 0 && my_rank == 0){
            for (int i = num_per_proc * comm_sz; i < table_size; i++) {
                new_result_dense_MPI[i] = 0;
                for (int j = 0; j < table_size; j++)
                    new_result_dense_MPI[i] += table[i][j] * original_vector[j];
            }
        }

        if(remainder > 0)
            MPI_Bcast(new_result_dense_MPI + num_per_proc * comm_sz, remainder, MPI_INT, 0, MPI_COMM_WORLD);

        if(i != num_rep -1)
            swap_vectors(&original_vector,&new_result_dense_MPI);
            
    }

    MPI_Barrier(MPI_COMM_WORLD);  
    finish = MPI_Wtime();

    comm_time2 = finish - start;
    if(my_rank == 0){
        printf("\n\nCommunication for Dense: %f sec\n",comm_time1);
        printf("Multiplying for Dense: %f sec\n", comm_time2);
        printf("Parallel-Dense: %f sec\n", comm_time1 + comm_time2);
    }

    if(my_rank == 0){
        if(sanity_check(new_result_dense_MPI,serial_dense_result,table_size))
            printf("Results of dense-parallel and serial-dense are matching and correct. Good job\n");
        else 
            printf("PROBLEM IN THE SANITY CHECK of serial implementation\n");
        free(*table);
        free(table);
        free(serial_dense_result);
        free(csr_table.row_idx);
        free(csr_table.col_idx);
        free(csr_table.values);
    }

    

    free(original_vector);
    free(local_table);
    free(local_result);
    free(new_result_dense_MPI);
    MPI_Finalize();
    return 0;
}