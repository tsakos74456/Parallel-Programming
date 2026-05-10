#!/bin/bash

EXEC=./main
OUTFILE=results_2_2_full.txt
RUNS=5  # number of testing

THREADS=(1 2 4 8 16)

# table size
SIZES=(1000 5000 10000 20000)
FIXED_REPS=7
FIXED_SPARSITY=50

# nuymber of rep
REPS_RANGE=(1 5 10 15 20)
FIXED_SIZE=10000

# percentage of zero values
SPARSITY_RANGE=(10  50  80 95)


echo "==============================================" >  $OUTFILE
echo "Exercise 2.2 – Complete Experimental Script"   >> $OUTFILE
echo "Runs per experiment: $RUNS"                    >> $OUTFILE
echo "==============================================" >> $OUTFILE
echo "" >> $OUTFILE

# ==================================================
# PHASE 1 — MATRIX SIZE SCALING
# ==================================================
echo "########## PHASE 1: MATRIX SIZE SCALING ##########" >> $OUTFILE

for T in "${THREADS[@]}"; do
  echo "" >> $OUTFILE
  echo "Threads = $T" >> $OUTFILE
  echo "----------------------------------------------" >> $OUTFILE

  for N in "${SIZES[@]}"; do
    echo "N = $N | Sparsity = $FIXED_SPARSITY% | Reps = $FIXED_REPS" >> $OUTFILE

    for ((i=1; i<=RUNS; i++)); do
      echo " Run $i:" >> $OUTFILE
      $EXEC $N $FIXED_SPARSITY $FIXED_REPS $T >> $OUTFILE
    done

    echo "" >> $OUTFILE
  done
done

# ==================================================
# PHASE 2 — REPETITIONS
# ==================================================
echo "########## PHASE 2: REPETITIONS ##########" >> $OUTFILE

for T in "${THREADS[@]}"; do
  echo "" >> $OUTFILE
  echo "Threads = $T | N = $FIXED_SIZE | Sparsity = $FIXED_SPARSITY%" >> $OUTFILE
  echo "----------------------------------------------" >> $OUTFILE

  for R in "${REPS_RANGE[@]}"; do
    echo "Repetitions = $R" >> $OUTFILE

    for ((i=1; i<=RUNS; i++)); do
      echo " Run $i:" >> $OUTFILE
      $EXEC $FIXED_SIZE $FIXED_SPARSITY $R $T >> $OUTFILE
    done

    echo "" >> $OUTFILE
  done
done

# ==================================================
# PHASE 3 — SPARSITY
# ==================================================
echo "########## PHASE 3: SPARSITY ##########" >> $OUTFILE

for T in "${THREADS[@]}"; do
  echo "" >> $OUTFILE
  echo "Threads = $T | N = $FIXED_SIZE | Reps = $FIXED_REPS" >> $OUTFILE
  echo "----------------------------------------------" >> $OUTFILE

  for S in "${SPARSITY_RANGE[@]}"; do
    echo "Sparsity = $S%" >> $OUTFILE

    for ((i=1; i<=RUNS; i++)); do
      echo " Run $i:" >> $OUTFILE
      $EXEC $FIXED_SIZE $S $FIXED_REPS $T >> $OUTFILE
    done

    echo "" >> $OUTFILE
  done
done

echo "DONE. Results saved in $OUTFILE"
