
#include "../include/mpi_multiplying_polynominals.h"

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


