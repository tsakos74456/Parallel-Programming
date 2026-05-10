
#include "../include/mpi_multiplying_polynominals.h"

int main(int argc, char** argv) {
    // Αρχικοποίηση του περιβάλλοντος MPI
    
    if(argc != 2){
        printf("error");
    }
    int degree_of_polynomial = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    
    int world_size , world_rank;
    // Λήψη του συνολικού αριθμού των processes
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Λήψη του ID (rank) του συγκεκριμένου process
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    // Δημιουργία 2 πλήρη τυχαία πολυωνύματων βαθμού n

   

///////////////////////////////////////////////////////////////////////////////////////////
    srand(time(NULL));
    // 1. Δημιουργία των πολυωνύμων εισόδου
    // Ένα πολυώνυμο n βαθμού έχει n+1 συντελεστές (από x^0 έως x^n)
    int size_n = degree_of_polynomial + 1;
    int size_r = 2 * degree_of_polynomial + 1; // Το αποτέλεσμα είναι βαθμού 2n
    int *P1 = (int *)malloc(size_n * sizeof(int));//πολυωνυμο 1
    int *P2 = (int *)malloc(size_n * sizeof(int));//πολυωνθμο 2
    int *R_serial = (int *)calloc(size_r, sizeof(int)); //Αποτελεσμα parallel
    int *R_parallel = (int *)calloc(size_r, sizeof(int));//Aποτέλεσμα serial
    int *local_res = (int *)calloc(size_r, sizeof(int));
    int *sendcounts = malloc(world_size * sizeof(int));// πινακας μεγεθους world_size
    int *displs = malloc(world_size * sizeof(int));// πινακας μεγεθους world_size
    int offset = 0;

    // Υπολογισμός του βασικού μεριδίου και του υπολοίπου
    int base = size_n / world_size; // Πόσα αναλογούν σίγουρα στον καθένα
    int temp = size_n % world_size;  // Πόσα περισσεύουν (το υπόλοιπο)
    for (int i = 0; i < world_size; i++) {

        int count_for_this_rank = base;
        
        // Αν υπάρχουν στοιχεία που περίσσεψαν, δίνουμε +1 στις πρώτες διεργασίες
        if (i < temp ) {
            count_for_this_rank = count_for_this_rank + 1;
        }
        
        // Αποθήκευση του αποτελέσματος στον πίνακα sendcounts
        sendcounts[i] = count_for_this_rank;
        displs[i] = offset;
        offset = offset + sendcounts[i];
    }

    // Κάθε διεργασία χρειάζεται χώρο για το δικό της κομμάτι του P1
    int local_count = sendcounts[world_rank];
    int *local_A = (int *)malloc(local_count * sizeof(int));
    double serial_time ;
    if (world_rank == 0) {
        for (int i = 0; i < size_n; i++) {
            P1[i] = get_random_non_zero();
            P2[i] = get_random_non_zero();
        }
        // Σειριακός υπολογισμός 
        double s_start = MPI_Wtime();
        serial(P1, P2, R_serial, degree_of_polynomial);
        double s_end = MPI_Wtime();
        serial_time = s_end - s_start;
    }

    MPI_Barrier(MPI_COMM_WORLD);
////////////////////////////////////////////////////////////////
    double start_total = MPI_Wtime();
// χρονος αποστολής
    double t_beg_comm = MPI_Wtime();
    MPI_Bcast( P2 , size_n ,MPI_INT , 0 , MPI_COMM_WORLD);
    MPI_Scatterv(P1 , sendcounts ,displs , MPI_INT , local_A , local_count , MPI_INT , 0 , MPI_COMM_WORLD);
    double t_end_comm = MPI_Wtime();

//  Χρόνος παράλληλου υπολογισμού
    double t_calc_start = MPI_Wtime();
    for (int i = 0; i < local_count; i++) {
        int global_idx = displs[world_rank] + i;
        for (int j = 0; j < size_n; j++) {
            local_res[global_idx + j] += local_A[i] * P2[j];
        }
    }
    double t_calc_end = MPI_Wtime();

// Χρόνος λήψης αποτελεσμάτων
    double t_recv_start = MPI_Wtime();
    MPI_Reduce(local_res, R_parallel, size_r, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double t_recv_end = MPI_Wtime();



    double end_total = MPI_Wtime();
///////////////////////////////////////////////////////////

  // Εκτύπωση αποτελεσμάτων από τον Rank 0
    if (world_rank == 0) {
        printf("\n=== Αποτελέσματα  ===\n");
        printf("Serial Execution Time:%f sec\n", serial_time);
        printf("Parallel Total Time:%f sec\n", end_total - start_total);
        printf("------------------------------\n");
        printf("Time Communicate :%f sec\n", t_end_comm - t_beg_comm);
        printf("Time Calculation:%f sec\n", t_calc_end - t_calc_start);
        printf("Time Receive:%f sec\n", t_recv_end - t_recv_start);
        printf("Speedup:%f sec\n",serial_time / (end_total - start_total) );
        // υπολογισμός Efficiency
        printf("Efficiency:%f\n", serial_time / (world_size * (end_total - start_total)));
        // Speedup calculation
        bool flag = true;
        for(int i = 0; i < size_r; i++) {
            if(R_parallel[i] != R_serial[i]) { 
                flag = false; 
                break; 
            }
        }
        printf("\nResult Validation parallel and serial are equal??: %s\n", flag ? "SUCCESS" : "FAILURE");

        free(P1); 
        free(R_serial); 
        free(R_parallel);
    }

    // Καθαρισμός όλων των διεργασιών
    free(P2); 
    free(local_A); 
    free(local_res);
    free(sendcounts); 
    free(displs);

    MPI_Finalize();
    return 0;
}