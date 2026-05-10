#!/bin/bash
export LC_ALL=C

EXEC="./test_main"
THREADS=(1 2 4 8 16 32)
SIZES=( 100 1000 10000 100000 )
OUTPUT_FILE="3.5.txt"

echo "=== MPI Benchmark Results ===" > $OUTPUT_FILE

for size in "${SIZES[@]}"
do
    echo "Size: $size"
    echo "=======================================" >> $OUTPUT_FILE
    echo "TABLE SIZE: $size" >> $OUTPUT_FILE
    echo "=======================================" >> $OUTPUT_FILE
    
    for t in "${THREADS[@]}" 
    do
        echo "  Running with $t threads..."
        
        # Αρχικοποίηση αθροισμάτων
        sum_serial=0
        sum_parallel=0
        sum_comm=0
        sum_calc=0
        sum_recv=0

        for attempt in {1..5}
        do
            echo "    Attempt $attempt..."
            result=$(mpiexec -f ../machines -n $t ./$EXEC $size)

            
            # Εξαγωγή τιμών
            v_serial=$(echo "$result" | grep "Serial Execution Time" | awk -F':' '{print $2}' | awk '{print $1}')
            v_parallel=$(echo "$result" | grep "Parallel Total Time" | awk -F':' '{print $2}' | awk '{print $1}')
            v_comm=$(echo "$result" | grep "Time Communicate" | awk -F':' '{print $2}' | awk '{print $1}')
            v_calc=$(echo "$result" | grep "Time Calculation" | awk -F':' '{print $2}' | awk '{print $1}')
            v_recv=$(echo "$result" | grep "Time Receive" | awk -F':' '{print $2}' | awk '{print $1}')

            # Πρόσθεση στο άθροισμα μέσω awk
            sum_serial=$(awk "BEGIN {print $sum_serial + $v_serial}")
            sum_parallel=$(awk "BEGIN {print $sum_parallel + $v_parallel}")
            sum_comm=$(awk "BEGIN {print $sum_comm + $v_comm}")
            sum_calc=$(awk "BEGIN {print $sum_calc + $v_calc}")
            sum_recv=$(awk "BEGIN {print $sum_recv + $v_recv}")
        done
        
        # Υπολογισμός Μέσων Όρων μέσω awk
        avg_serial=$(awk "BEGIN {print $sum_serial / 4}")
        avg_parallel=$(awk "BEGIN {print $sum_parallel / 4}")
        avg_comm=$(awk "BEGIN {print $sum_comm / 4}")
        avg_calc=$(awk "BEGIN {print $sum_calc / 4}")
        avg_recv=$(awk "BEGIN {print $sum_recv / 4}")

        # Υπολογισμός Speedup και Efficiency
        avg_speedup=$(awk "BEGIN {print $avg_serial / $avg_parallel}")
        avg_efficiency=$(awk "BEGIN {print $avg_speedup / $t}")

        # Εγγραφή των μέσων όρων στο αρχείο
        {
            echo "Threads: $t"
            echo "------------------------------"
            printf "Avg Serial Time:      %.8e sec\n" "$avg_serial"
            printf "Avg Parallel Total:   %.8e sec\n" "$avg_parallel"
            printf "Avg Communication:    %.8e sec\n" "$avg_comm"
            printf "Avg Calculation:      %.8e sec\n" "$avg_calc"
            printf "Avg Receive:          %.8e sec\n" "$avg_recv"
            printf "Avg Speedup:          %.8e\n" "$avg_speedup"
            printf "Avg Efficiency:       %.8e\n" "$avg_efficiency"
            echo "------------------------------"
            echo ""
        } >> $OUTPUT_FILE
    done
done

echo "Benchmarks completed! Check $OUTPUT_FILE for averages."