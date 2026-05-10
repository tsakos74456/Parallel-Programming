
import matplotlib.pyplot as plt

# Common x-axis (threads)
threads = [1, 2, 4, 8, 16]

# -------------------------
# SPEEDUP ARRAYS
# -------------------------
speedup_10 =    [1.00, 0.012926, 0.006516, 0.001024, 0.001669 ]
speedup_100 =   [1.00, 0.1159, 0.2857, 0.02713, 0.08675]
speedup_1000 =  [1.00, 0.8934, 1.6289, 1.9483, 1.8790]
speedup_10000 = [1.00, 1.0558, 2.0060, 2.1986, 2.1796]
speedup_100000 =[1.00, 1.0219, 1.9570, 2.1488, 2.1356]

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
efficiency_10 =    [1.00, 0.006463, 0.001629, 0.000128, 0.000104]
efficiency_100 =   [1.00, 0.0579, 0.07143, 0.003391, 0.00542]
efficiency_1000 =  [1.00, 0.4467, 0.4072, 0.2435, 0.1174]
efficiency_10000 = [1.00, 0.5279, 0.5015, 0.2748, 0.1362]
efficiency_100000 =[1.00, 0.5109, 0.4893, 0.2686, 0.1335]

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
