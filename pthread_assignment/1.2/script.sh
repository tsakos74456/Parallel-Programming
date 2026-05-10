#!/bin/bash

OUT="results.txt"
echo "Benchmark results" >> "$OUT"
echo "=================" >> "$OUT"
echo "" >> "$OUT"

# Programs in the exact order you want
PROGRAMS=(
    "./main_mutex"
    "./main_rwlock"
    "./main_atomic"
)

THREADS=(1 2 4 8 16)

make clean
make

for PROG in "${PROGRAMS[@]}"; do
    
    echo "Running $PROG ..." | tee -a "$OUT"
    echo "--------------------------" >> "$OUT"

    for T in "${THREADS[@]}"; do
        echo "Threads: $T" >> "$OUT"
        $PROG $T >> "$OUT"
        echo "" >> "$OUT"
    done

    echo "" >> "$OUT"
done

echo "DONE! Results saved to results.txt"
