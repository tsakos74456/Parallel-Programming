import matplotlib.pyplot as plt

# Common x-axis (threads)
threads = [1, 2, 4, 8, 16]

# -------------------------
# SPEEDUP ARRAYS
# -------------------------
speedup_10 =    [1.00, 0.002695, 0.002674, 0.001947, 0.001279]
speedup_100 =   [1.00, 0.12,     0.14,     0.10,     0.07]
speedup_1000 =  [1.00, 0.91,     0.89,     1.49,     1.79]
speedup_10000 = [1.00, 0.86,     1.10,     1.56,     1.64]
speedup_100000 =[1.00, 0.85,     1.42,     1.62,     1.72]

speedups = {
    "Degree 10": speedup_10,
    "Degree 100": speedup_100,
    "Degree 1000": speedup_1000,
    "Degree 10000": speedup_10000,
    "Degree 100000": speedup_100000
}

# -------------------------
# EFFICIENCY ARRAYS
# -------------------------
efficiency_10 =    [1.00, 0.001348, 0.000668, 0.000243, 0.000080]
efficiency_100 =   [1.00, 0.06,     0.03,     0.01,     0.00]
efficiency_1000 =  [1.00, 0.46,     0.22,     0.19,     0.11]
efficiency_10000 = [1.00, 0.43,     0.28,     0.20,     0.10]
efficiency_100000 =[1.00, 0.42,     0.36,     0.20,     0.11]

efficiencies = {
    "Degree 10": efficiency_10,
    "Degree 100": efficiency_100,
    "Degree 1000": efficiency_1000,
    "Degree 10000": efficiency_10000,
    "Degree 100000": efficiency_100000
}


# -------------------------
# PLOT SPEEDUP
# -------------------------
plt.figure(figsize=(10,6))

for label, data in speedups.items():
    plt.plot(threads, data, marker='o', linewidth=2, label=label)

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
    plt.plot(threads, data, marker='o', linewidth=2, label=label)

plt.title("Αποδοτικότητα Παράλληλης Εκτέλεσης (Efficiency)")
plt.xlabel("Threads (P)")
plt.ylabel("Efficiency")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
