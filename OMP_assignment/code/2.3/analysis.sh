#!/bin/bash

# Ορισμός του ονόματος του εκτελέσιμου (άλλαξέ το αν το δικό σου λέγεται αλλιώς)
EXECUTABLE="./test_main"
declare -A serial_results
declare -A parallel_results
# Έλεγχος αν υπάρχει το εκτελέσιμο
if [ ! -f "$EXECUTABLE" ]; then
    echo "Σφάλμα: Το $EXECUTABLE δεν βρέθηκε! Κάνε compile (make) πρώτα."
    exit 1
fi

# Ρυθμίσεις 100000000
N_VALUES=(100 1000 10000 100000 1000000   )   # 10^7 και 10^8
P_VALUES=(1 2 4 8 16 )        # Αριθμός νημάτων
ITERATIONS=5                   # 5 επαναλήψεις

# Αρχείο εξόδου
OUTPUT_FILE="2.3.txt"
> "$OUTPUT_FILE"

echo "--- ΕΝΑΡΞΗ ΠΕΙΡΑΜΑΤΩΝ: ---" | tee -a "$OUTPUT_FILE"

echo "=== SERIAL EXECUTION ===" | tee -a "$OUTPUT_FILE"

for n in "${N_VALUES[@]}"; do
    echo " -------------------------" | tee -a "$OUTPUT_FILE"
    sum_time=0
    
    for (( i=1; i<=$ITERATIONS; i++ )); do
        echo "--Running: N = $n, mode = serial, Attempt $i--" | tee -a "$OUTPUT_FILE"
        
  
        output=$($EXECUTABLE "$n" "serial" 1 | tee -a "$OUTPUT_FILE")
        current_time=$(echo "$output" | grep "Serial Time" | cut -d':' -f2)
        
     
        sum_time=$(awk "BEGIN {print $sum_time + $current_time}")
    done
    
   
    avg_time=$(awk "BEGIN {print $sum_time / $ITERATIONS}")
    echo ">>> AVERAGE SERIAL TIME for (N=$n): $avg_time <<<" | tee -a "$OUTPUT_FILE"
   
    serial_results["$n"]=$avg_time
done

echo "=== PARALLEL EXECUTION ===" | tee -a "$OUTPUT_FILE"

for n in "${N_VALUES[@]}"; do
    for t in "${P_VALUES[@]}"; do
        echo " -------------------------" | tee -a "$OUTPUT_FILE"
        sum_time_parallel=0
        
        for (( i=1; i<=$ITERATIONS; i++ )); do
            echo "--Running: N = $n, mode = parallel, Threads = $t, Attempt $i--" | tee -a "$OUTPUT_FILE"
            
           
           
            output=$($EXECUTABLE "$n" "parallel" "$t" | tee -a "$OUTPUT_FILE")
           
            current_time=$(echo "$output" | grep -i "Time" | cut -d':' -f2 | xargs)
            
            if [[ ! -z "$current_time" ]]; then
                
                sum_time_parallel=$(awk "BEGIN {print $sum_time_parallel + $current_time}")
            fi
        done
        
        
        avg_parallel_time=$(awk "BEGIN {print $sum_time_parallel / $ITERATIONS}")
        
        
        echo ">>> AVERAGE PARALLEL TIME (N=$n, P=$t): $avg_parallel_time <<<" | tee -a "$OUTPUT_FILE"
        # Αποθήκευση στον πίνακα με σύνθετο κλειδί "N,Threads"
        parallel_results["$n,$t"]=$avg_parallel_time
    done
done


# === ΕΜΦΑΝΙΣΗ ΣΥΓΚΕΝΤΡΩΤΙΚΟΥ ΠΙΝΑΚΑ ===
echo -e "\n=== ΣΥΓΚΕΝΤΡΩΤΙΚΑ ΑΠΟΤΕΛΕΣΜΑΤΑ ===" | tee -a "$OUTPUT_FILE"
# Επικεφαλίδα πίνακα
printf "%-12s | %-8s | %-12s | %-8s | %-10s\n" "N" "Threads" "Avg Time" "Speedup" "Efficiency" | tee -a "$OUTPUT_FILE"
echo "----------------------------------------------------------------------" | tee -a "$OUTPUT_FILE"

for n in "${N_VALUES[@]}"; do
   
    s_time=${serial_results["$n"]}
    
    # Εκτύπωση Σειριακής Γραμμής (Speedup=1, Efficiency=1)
    LC_ALL=C printf "%-12s | %-8s | %-12.6f | %-8.4f | %-10.4f\n" "$n" "1 (Ser)" "$s_time" "1.00" "1.00" | tee -a "$OUTPUT_FILE"
    
    for t in "${P_VALUES[@]}"; do
        p_time=${parallel_results["$n,$t"]}
        
        # Υπολογισμός Speedup και Efficiency με awk
        # S = s_time / p_time
        # E = S / t
        results=$(awk "BEGIN { 
            s = $s_time / $p_time; 
            e = s / $t; 
            printf \"%.2f|%.2f\", s, e 
        }")
        
        # Διαχωρισμός των αποτελεσμάτων της awk
        speedup=$(echo $results | cut -d'|' -f1)
        efficiency=$(echo $results | cut -d'|' -f2)
        
        # Εκτύπωση Παράλληλης Γραμμής
    LC_ALL=C printf "%-12d | %-8s | %-12.6f | %-8.6f | %-10.6f\n" "$n" "Par $t "  "$p_time" "$speedup" "$efficiency" | tee -a "$OUTPUT_FILE"
    done
    echo "----------------------------------------------------------------------" | tee -a "$OUTPUT_FILE"
done


# 