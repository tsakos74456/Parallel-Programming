
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include "omp_multiplying_polynomials.h"
#include <omp.h>


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

void parallel(int *P1, int *P2, int *R, int n, int num_threads) {
    // Το μέγεθος του αποτελέσματος είναι 2*n + 1
    int total_elements = 2 * n + 1;

    #pragma omp parallel for num_threads(num_threads) schedule(dynamic)
    // Υπολογισμός των στοιχείων που αναλογούν σε αυτό το νήμα
    for (int k = 0; k < total_elements; k++) {
        int sum = 0;
        for (int i = 0; i <= n; i++) {
            int j = k - i;
            // Έλεγχος ορίων για τα πολυώνυμα P1 και P2
            if (j >= 0 && j <= n) {
                sum += P1[i] * P2[j];
            }
        }
        R[k] = sum;
    }

}
