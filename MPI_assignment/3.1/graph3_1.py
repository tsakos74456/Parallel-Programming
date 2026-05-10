
import matplotlib.pyplot as plt

# Common x-axis (threads)
Process = [1, 2, 4, 8 ,16, 32, 64]

# -------------------------
# SPEEDUP ARRAYS
# -------------------------
speedup_100 =    [ 1.00 , 0.561 , 0.932 , 0.0026 , 0.0012 , 0.00096 , 0.00009 ]
speedup_1000 =   [ 1.00 , 1.385 , 3.007 , 0.122 , 0.148 , 0.06 , 0.003 ]
speedup_10000 =  [ 1.00 , 2.21 , 3.506 , 3.82 , 5.404 , 5.47 ,  4.82  ]
speedup_100000 = [ 1.00 , 2.38 , 3.52 , 6.241 , 13.491 , 25.705 , 40.23]


speedups = {
    "Degree 100": speedup_100,
    "Degree 1000": speedup_1000,
    "Degree 10000": speedup_10000,
    "Degree 100000": speedup_100000,
   
}

# -------------------------
# EFFICIENCY ARRAYS
# -------------------------
efficiency_100 =    [ 1.00 , 0.281 , 0.233 , 0.00033 , 0.00008 , 0.00003 , 0.000014 ]
efficiency_1000 =   [ 1.00 , 0.693 , 0.752 , 0.015 , 0.0093 , 0.0019 , 0.00046 ]
efficiency_10000 =  [ 1.00 , 1.11 , 0.877 , 0.478 , 0.338 , 0.171 , 0.075]
efficiency_100000 = [ 1.00 , 1.19 , 0.88 , 0.78 , 0.843 , 0.803 , 0.63]


efficiencies = {
    "Degree 100": efficiency_100,
    "Degree 1000": efficiency_1000,
    "Degree 10000": efficiency_10000,
    "Degree 100000": efficiency_100000,

}


# -------------------------
# PLOT SPEEDUP
# -------------------------
plt.figure(figsize=(10,6))

for label, data in speedups.items():
    plt.plot(Process, data, marker='o', linewidth=2, label=label)

plt.title("Επιτάχυνση Παράλληλης Εκτέλεσης (Speedup)")
plt.xlabel("Process (P)")
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
    plt.plot(Process, data, marker='o', linewidth=2, label=label)

plt.title("Αποδοτικότητα Παράλληλης Εκτέλεσης (Efficiency)")
plt.xlabel("Process (P)")
plt.ylabel("Efficiency")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()
