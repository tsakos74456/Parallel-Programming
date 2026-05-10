#!/bin/bash

# Output file
OUTFILE="barrier_results.txt"

# Thread counts you want to test
THREADS=(1 2 4 8 16)

# Executable names (όπως στο Makefile)
PTHREAD_BIN=barrier_pthread
MUTEX_COND_BIN=barrier_mutex_cond
SENSE_BIN=barrier_sense_reversal_centralized

# Clear old output
echo "=== Barrier Benchmark Results ===" > "$OUTFILE"
echo "Generated at: $(date)" >> "$OUTFILE"
echo >> "$OUTFILE"

# Build binaries
make

# Helper function to log and display at the same time
log() {
    echo -e "$1" | tee -a "$OUTFILE"
}

log "==============================="
log " Running PTHREAD BARRIER TESTS "
log "==============================="

for t in "${THREADS[@]}"; do
    log "\n--- Threads: $t ---"
    ./$PTHREAD_BIN $t 2>&1 | tee -a "$OUTFILE"
done

log "\n===================================="
log " Running MUTEX + COND BARRIER TESTS "
log "===================================="

for t in "${THREADS[@]}"; do
    log "\n--- Threads: $t ---"
    ./$MUTEX_COND_BIN $t 2>&1 | tee -a "$OUTFILE"
done

log "\n=============================================="
log " Running SENSE REVERSAL CENTRALIZED BARRIERS "
log "=============================================="

for t in "${THREADS[@]}"; do

    log "\n--- Threads: $t ---"
    ./$SENSE_BIN $t 2>&1 | tee -a "$OUTFILE"
done

log "\n=== DONE ==="
