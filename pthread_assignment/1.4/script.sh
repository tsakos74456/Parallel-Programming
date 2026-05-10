#!/bin/bash

# ===============================
# CONFIGURATION
# ===============================

TABLE_SIZE=1000             # σταθερό table size
TRANSACTIONS=100000             # σταθερός αριθμός συναλλαγών
PERCENTS=(10 40 60 80)          # εδώ αλλάζουν τα % read queries

EXEC=./bank

LOCK_TYPES=(0 1 2 3)
THREADS=(1 2 4 8 16)

OUTFILE="results_percent.txt"

# ===============================
# START
# ===============================
echo "=== BANK BENCHMARK (varying percent) ===" > "$OUTFILE"
date >> "$OUTFILE"
echo "" >> "$OUTFILE"

for pct in "${PERCENTS[@]}"; do
    echo "=== READ PERCENT: $pct% ===" | tee -a "$OUTFILE"
    echo "" >> "$OUTFILE"

    for lock in "${LOCK_TYPES[@]}"; do
        echo "--- LOCK TYPE: $lock ---" | tee -a "$OUTFILE"

        for t in "${THREADS[@]}"; do
            echo "Running: TABLE=$TABLE_SIZE TRANS=$TRANSACTIONS PCT=$pct LOCK=$lock THREADS=$t" | tee -a "$OUTFILE"

            # Correct execution
            $EXEC "$TABLE_SIZE" "$TRANSACTIONS" "$pct" "$lock" "$t" >> "$OUTFILE" 2>&1

            echo "" >> "$OUTFILE"
        done

        echo "" >> "$OUTFILE"
    done

    echo "---------------------------------------------" >> "$OUTFILE"
    echo "" >> "$OUTFILE"
done

echo "=== DONE ===" | tee -a "$OUTFILE"
