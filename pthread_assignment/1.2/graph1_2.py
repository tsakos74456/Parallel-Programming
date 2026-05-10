import matplotlib.pyplot as plt

# ============================
#  ΔΩΣΕ ΕΔΩ ΤΑ ΔΙΚΑ ΣΟΥ ΔΕΔΟΜΕΝΑ
# ============================

threads = [1, 2, 4, 8, 16]

eff_mutex   = [1.00, 0.2, 0.17, 0.069, 0.033]
eff_rwlock  = [1.00, 0.164, 0.102, 0.066, 0.00091]
eff_atomic  = [1.00, 0.1417, 0.089, 0.032, 0.0189]

labels = [
    ("Mutex", eff_mutex),
    ("RWLock", eff_rwlock),
    ("Atomic", eff_atomic)
]

# ============================
#  DRAW
# ============================

plt.figure(figsize=(10, 6))

for label, eff in labels:
    plt.plot(
        threads, eff, 
        marker='o', 
        linewidth=2,
        markersize=7,
        label=label
    )

plt.title("Speedup", fontsize=18)
plt.xlabel("Νήματα", fontsize=14)
plt.ylabel("Speedup", fontsize=14)
plt.grid(True, linestyle="--", alpha=0.5)
plt.xticks(threads)
plt.legend(fontsize=12)

plt.tight_layout()
plt.savefig("efficiency_plot.png", dpi=200)
plt.show()
