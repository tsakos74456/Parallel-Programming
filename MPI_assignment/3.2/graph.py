import matplotlib.pyplot as plt

# Common x-axis (threads)
threads = [1, 2, 4, 8]

# -------------------------
# SPEEDUP ARRAYS
# -------------------------
speedups = {
    "elements per row:1000 (dense)":  [1.00, 1.62, 2.671, 0.873],
    "elements per row:1000 (CSR)":    [1.00, 1.17, 1.32, 0.73],

    "elements per row:5000 (dense)":  [1.00, 1.63, 2.70, 0.96],
    "elements per row:5000 (CSR)":    [1.00, 1.16, 1.29, 0.79],

    "elements per row:10000 (dense)": [1.00, 1.64, 2.70, 0.97],
    "elements per row:10000 (CSR)":   [1.00, 1.20, 1.32, 0.81],

    "elements per row:20000 (dense)": [1.00, 1.63, 2.70, 0.96],
    "elements per row:20000 (CSR)":   [1.00, 1.21, 1.34, 0.81]
}


# -------------------------
# EFFICIENCY ARRAYS
# -------------------------
efficiencies = {
    "elements per row:1000 (dense)":  [1.00, 0.81, 0.67, 0.11],
    "elements per row:1000 (CSR)":    [1.00, 0.59, 0.33, 0.09],

    "elements per row:5000 (dense)":  [1.00, 0.82, 0.68, 0.12],
    "elements per row:5000 (CSR)":    [1.00, 0.58, 0.323, 0.10],

    "elements per row:10000 (dense)": [1.00, 0.82, 0.68, 0.12],
    "elements per row:10000 (CSR)":   [1.00, 0.60, 0.68, 0.10],

    "elements per row per row:20000 (dense)": [1.00, 0.82, 0.68, 0.12],
    "elements per row:20000 (CSR)":   [1.00, 0.61, 0.34, 0.10]
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
plt.xlabel("Processes (P)")
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
plt.xlabel("Processes (P)")
plt.ylabel("Efficiency")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
