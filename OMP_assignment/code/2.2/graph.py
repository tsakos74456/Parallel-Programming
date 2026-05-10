import matplotlib.pyplot as plt

# Common x-axis (threads)
threads = [1, 2, 4, 8, 16]

# -------------------------
# SPEEDUP ARRAYS
# -------------------------
speedup_1_dense  = [1.00, 1.892, 3.665, 3.96, 3.96]
speedup_1_csr    = [1.00, 1.94,  3.70,  5.17, 5.43]

speedup_5_dense  = [1.00, 1.886, 3.66,  3.951, 3.94]
speedup_5_csr    = [1.00, 1.916, 3.72,  4.85,  4.93]

speedup_10_dense = [1.00, 1.946, 3.776, 4.11,  4.08]
speedup_10_csr   = [1.00, 1.91,  3.71,  4.51, 4.545]

speedup_15_dense = [1.00, 1.956, 3.78,  4.11,  4.08]
speedup_15_csr   = [1.00, 1.91,  3.71,  4.35,  4.40]

speedup_20_dense = [1.00, 1.955, 3.81,  4.133, 4.09]
speedup_20_csr   = [1.00, 1.912, 3.71,  4.24,  4.25]

speedups = {
    "elements per row:1000(dense)": speedup_1_dense,
    "elements per row:1000(CSR)": speedup_1_csr,
    "selements per row:5000(dense)": speedup_10_dense,
    "elements per row:5000(CSR)": speedup_10_csr,
    "elements per row:10000(dense)": speedup_15_dense,
    "elements per row:10000(CSR)": speedup_15_csr,
    "elements per row:20000(dense)": speedup_20_dense,
    "elements per row:20000(CSR)": speedup_20_csr

}

# -------------------------
# EFFICIENCY ARRAYS
# -------------------------
efficiency_1_dense  = [1.00, 0.946, 0.92,  0.495, 0.248]
efficiency_1_csr    = [1.00, 0.97,  0.925, 0.646, 0.34]

efficiency_5_dense  = [1.00, 0.943, 0.915, 0.49,  0.246]
efficiency_5_csr    = [1.00, 0.958, 0.93,  0.61,  0.31]

efficiency_10_dense = [1.00, 0.973, 0.944, 0.514, 0.255]
efficiency_10_csr   = [1.00, 0.955, 0.9275,0.564, 0.28]

efficiency_15_dense = [1.00, 0.978, 0.945, 0.514, 0.255]
efficiency_15_csr   = [1.00, 0.955, 0.93,  0.544, 0.275]

efficiency_20_dense = [1.00, 0.977, 0.953, 0.517, 0.256]
efficiency_20_csr   = [1.00, 0.956, 0.93,  0.53,  0.266]

efficiencies = {
    "elements per row:1000(dense)": efficiency_1_dense,
    "elements per row:1000(CSR)": efficiency_1_csr,
    "selements per row:5000(dense)": efficiency_10_dense,
    "elements per row:5000(CSR)": efficiency_10_csr,
    "elements per row:10000(dense)": efficiency_15_dense,
    "elements per row:10000(CSR)": efficiency_15_csr,
    "elements per row:20000(dense)": efficiency_20_dense,
    "elements per row:20000(CSR)": efficiency_20_csr

}


# -------------------------
# PLOT SPEEDUP
# -------------------------

plt.figure(figsize=(10,6))

for label, data in speedups.items():
    ls = '--' if '(CSR)' in label else '-'   
    mk = 's'  if '(CSR)' in label else 'o'   
    plt.plot(threads, data, marker=mk, linestyle=ls, linewidth=2, label=label)

plt.title("Επιτάχυνση Παράλληλης Εκτέλεσης (Speedup)")
plt.xlabel("Threads (P)")
plt.ylabel("Speedup")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()


# -------------------------
# PLOT EFFICIENCY
# -------------------------
plt.figure(figsize=(10,6))

for label, data in efficiencies.items():
    ls = '--' if '(CSR)' in label else '-'
    mk = 's'  if '(CSR)' in label else 'o'
    plt.plot(threads, data, marker=mk, linestyle=ls, linewidth=2, label=label)

plt.title("Αποδοτικότητα Παράλληλης Εκτέλεσης (Efficiency)")
plt.xlabel("Threads (P)")
plt.ylabel("Efficiency")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
