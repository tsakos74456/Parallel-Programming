
import matplotlib.pyplot as plt

# Common x-axis (threads)
threads = [1, 2, 4, 8, 16]

# -------------------------
# SPEEDUP ARRAYS
# -------------------------
speedup_100 =        [1.00, 0.586, 0.3287, 0.17803, 0.04791]
speedup_1000 =       [1.00, 0.286, 0.232, 0.0654, 0.128]
speedup_10000 =      [1.00, 1.1035, 1.05, 0.744, 0.894]
speedup_100000 =     [1.00, 2.575, 3.2, 2.897, 1.89]
speedup_1000000 =    [1.00, 1.57, 1.99, 2.166, 3.18]
speedup_10000000 =   [1.00, 1.66, 2.125, 2.78, 3.39]
speedup_100000000 =  [1.00, 1.69, 2.067, 3.01, 3.589]

speedups = {
    "Degree 100": speedup_100,
    "Degree 1000": speedup_1000,
    "Degree 10000": speedup_10000,
    "Degree 100000": speedup_100000,
    "Degree 1000000": speedup_1000000,
    "Degree 10000000": speedup_10000000,
    "Degree 100000000": speedup_100000000
}

# -------------------------
# EFFICIENCY ARRAYS
# -------------------------
efficiency_100 =        [1.00, 0.293, 0.082, 0.0223, 0.00294]
efficiency_1000 =       [1.00, 0.143, 0.058, 0.00818, 0.008]
efficiency_10000 =      [1.00, 0.552, 0.263, 0.093, 0.0559]
efficiency_100000 =     [1.00, 1.288, 0.8, 0.362, 0.118]
efficiency_1000000 =    [1.00, 0.785, 0.498, 0.271, 0.199]
efficiency_10000000 =   [1.00, 0.83, 0.531, 0.348, 0.212]
efficiency_100000000 =  [1.00, 0.845, 0.515, 0.377, 0.2243]

efficiencies = {
    "Degree 100": efficiency_100,
    "Degree 1000": efficiency_1000,
    "Degree 10000": efficiency_10000,
    "Degree 100000": efficiency_100000,
    "Degree 1000000": efficiency_1000000,
    "Degree 10000000": efficiency_10000000,
    "Degree 100000000": efficiency_100000000
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
