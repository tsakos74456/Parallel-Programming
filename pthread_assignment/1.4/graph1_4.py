import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16]

eff_coarse_mutex  = [1.00, 0.124, 0.0360, 0.0129, 0.00277]
eff_coarse_rwlock = [1.00, 0.0785, 0.0189, 0.00515, 0.000102]
eff_fine_mutex    = [1.00, 0.409, 0.108, 0.0334, 0.00874]
eff_fine_rwlock   = [1.00, 0.397, 0.139, 0.0463, 0.0127]

labels = [
    ("Coarse Mutex", eff_coarse_mutex),
    ("Coarse RWLock", eff_coarse_rwlock),
    ("Fine Mutex", eff_fine_mutex),
    ("Fine RWLock", eff_fine_rwlock)
]

plt.figure(figsize=(10, 6))

for label, eff in labels:
    plt.plot(threads, eff, marker='o', linewidth=2, markersize=7, label=label)

plt.title("Efficiency", fontsize=16)
plt.xlabel("Threads", fontsize=14)
plt.ylabel("Efficiency", fontsize=14)
plt.grid(True, linestyle="--", alpha=0.5)
plt.xticks(threads)
plt.legend(fontsize=12)

plt.tight_layout()
plt.savefig("efficiency_plot_case2.png", dpi=200)
plt.show()
