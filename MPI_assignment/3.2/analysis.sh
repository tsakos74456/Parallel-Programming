#!/bin/bash

EXEC=./matrix_mul
MACHINES=../machines

RUNS=5

# -----------------------------
# Παράμετροι Πειραμάτων
# -----------------------------

PROCS=(1 2 4 8 16 32)
SIZES=(1000 5000 10000 20000)
SPARSITY=(0 50 80 95 99)
REPS=(1 5 10 20)

OUTFILE=results_3_2.txt
echo "MPI Sparse Matrix-Vector Multiplication – Exercise 3.2" > $OUTFILE
echo "===============================================" >> $OUTFILE
echo "" >> $OUTFILE

# -----------------------------
# BUILD
# -----------------------------
make clean && make
if [ $? -ne 0 ]; then
  echo "Build failed!"
  exit 1
fi

# =================================================
# EXPERIMENT 1: Scaling (for each matrix size)
# =================================================
echo "### EXPERIMENT 1: Scaling (CSR)" >> $OUTFILE
echo "Fixed: sparsity=50%, reps=10" >> $OUTFILE
echo "" >> $OUTFILE

for N in "${SIZES[@]}"; do
  echo "--- Matrix size N=$N ---" >> $OUTFILE
  for P in "${PROCS[@]}"; do
    for ((r=1; r<=RUNS; r++)); do
      echo -e " Run $r | N=$N | P=$P" >> $OUTFILE
      mpiexec -f $MACHINES -n $P $EXEC $N 50 10 >> $OUTFILE
    done
  done
  echo "" >> $OUTFILE
done

# =================================================
# EXPERIMENT 2: CSR vs Dense (for each matrix size)
# =================================================
echo "### EXPERIMENT 2: CSR vs Dense" >> $OUTFILE
echo "Fixed: N=10000, reps=10" >> $OUTFILE
echo "" >> $OUTFILE

for S in "${SPARSITY[@]}"; do
  echo "--- Matrix size N=10000 ---" >> $OUTFILE
  for P in "${PROCS[@]}"; do
    for ((r=1; r<=RUNS; r++)); do
      echo -e " Run $r | P=$P | sparsity=$S" >> $OUTFILE
      mpiexec -f $MACHINES -n $P $EXEC 10000 $S 10 >> $OUTFILE
    done
  done
  echo "" >> $OUTFILE
done

# =================================================
# EXPERIMENT 3: Repetitions effect (for each matrix size)
# =================================================
echo "### EXPERIMENT 3: Repetitions Effect" >> $OUTFILE
echo "Fixed: N=10000, sparsity=50%" >> $OUTFILE
echo "" >> $OUTFILE

for R in "${REPS[@]}"; do
  echo "--- Matrix size N=10000 ---" >> $OUTFILE
  for P in "${PROCS[@]}"; do
    for ((r=1; r<=RUNS; r++)); do
      echo -e " Run $r | P=$P | reps=$R" >> $OUTFILE
      mpiexec -f $MACHINES -n $P $EXEC 10000 50 $R >> $OUTFILE
    done
  done
  echo "" >> $OUTFILE
done

echo "DONE." >> $OUTFILE
