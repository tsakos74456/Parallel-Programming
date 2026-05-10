
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





void *thread_worker(void *arg){
    ThreadData *data = (ThreadData *)arg;
    
    int n = data->n;
    int size_r = 2 * n + 1;
    
    // Κατανομή εργασίας: Υπολογισμός ενός υποσυνόλου συντελεστών R[k]
    int chunk_size = size_r / data->num_threads; // Αριθμός συντελεστών ανά νήμα
    int start_k = data->thread_id * chunk_size; // Αρχικός δείκτης για το τρέχον νήμα
    int end_k = (data->thread_id == data->num_threads - 1) ? size_r : start_k + chunk_size; // Τελικός δείκτης (για το τελευταίο νήμα μπορεί να είναι μεγαλύτερος)

    // Ο κάθε συντελεστής R[k] υπολογίζεται ανεξάρτητα
    for (int k = start_k; k < end_k; k++) {
        // Υπολογισμός R[k] = Sum( P1[i] * P2[j] ) όπου i+j=k
        for (int i = 0; i <= n; i++) {
            int j = k - i;
            
            // Έλεγχος αν ο δείκτης j είναι εντός των ορίων του P2
            if (j >= 0 && j <= n) {
                data->R[k] += data->P1[i] * data->P2[j];
            }
        }
    }

    pthread_exit(NULL);

}


int get_random_non_zero() {
    int val = rand() % 20 + 1; // 1 to 20
    if (val <= 10) {
        return -(val); // -10 to -1
    } else {
        return val - 10; // 1 to 10
    }
}

void serial(int *P1, int *P2, int *R, int n){
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            R[i + j] += P1[i] * P2[j];
        }
    }
   

}

void parallel( int *P1, int *P2, int *R, int n, int num_threads){
    pthread_t threads[num_threads];//δημιουργία νημάτων
    ThreadData thread_data[num_threads];//δομή δεδομένων για κάθε νήμα

    for (int i = 0 ; i <  num_threads  ; i++){
        thread_data[i].P1 = P1;
        thread_data[i].P2 = P2;
        thread_data[i].R = R;
        thread_data[i].n = n;
        thread_data[i].num_threads = num_threads;
        thread_data[i].thread_id = i;

        pthread_create(&threads[i], NULL, thread_worker,(void *)&thread_data[i]  );
    }

    for (int i = 0 ; i <  num_threads  ; i++){
       pthread_join(threads[i], NULL);
    }
    
    // for (int k = 0; k <= 2*n; k++) {
    //     printf("R[%d] = %d\n", k, R[k]);
    // }


}
