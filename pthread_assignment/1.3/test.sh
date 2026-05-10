#!/bin/bash

# Ορισμός του ονόματος του εκτελέσιμου (σύμφωνα με το Makefile: TARGET = test_main)
EXECUTABLE="./test_main"

# Ορισμός των τιμών για τον αριθμό των στοιχείων (N)
N_VALUES=(10 100 1000 10000 100000 1000000 10000000 100000000)

# ΟΡΙΣΜΟΣ ΤΟΥ ΑΡΧΕΙΟΥ ΕΞΟΔΟΥ
OUTPUT_FILE="1.3.txt"

# Καθαρισμός του αρχείου εξόδου πριν την εκτέλεση
> "$OUTPUT_FILE"

echo "---" | tee -a "$OUTPUT_FILE"



for N in "${N_VALUES[@]}"; do
        (
            echo "Εκτέλεση $i με n=${N}"
            
            # Εκτέλεση του προγράμματος με το τρέχον όρισμα n
            # Η έξοδος του προγράμματος (Timings, Speedup, Verification) διοχετεύεται στο tee.
            "$EXECUTABLE" "$N"
            
            echo "---"
        ) | tee -a "$OUTPUT_FILE"
done

echo "--- Όλες οι εκτελέσεις ολοκληρώθηκαν. ---" | tee -a "$OUTPUT_FILE"