import matplotlib.pyplot as plt
import numpy as np

# --- Data ---
N = np.array([10, 100, 1000, 10000, 100000, 
              1_000_000, 10_000_000, 100_000_000])

serial_time = np.array([0.000001, 0.000003, 0.000035, 0.000337, 
                        0.002090, 0.008017, 0.080012, 0.779430])

parallel_time = np.array([0.000826, 0.000715, 0.000793, 0.000859, 
                          0.001283, 0.004409, 0.037612, 0.307358])

# --- Calculations ---
speedup = serial_time / parallel_time
efficiency = speedup / 4   # 4 threads

# --- Plot: Speedup ---
plt.figure(figsize=(8, 5))
plt.plot(N, speedup, marker='o')
plt.xscale('log')
plt.xlabel("N (elements)")
plt.ylabel("Speedup")
plt.title("Speedup with 4 threads, 1 for each table")
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.show()

# --- Plot: Efficiency ---
plt.figure(figsize=(8, 5))
plt.plot(N, efficiency, marker='o')
plt.xscale('log')
plt.xlabel("N (elements)")
plt.ylabel("Efficiency")
plt.title("Efficiency with 4 threads, 1 for each table")
plt.grid(True, which="both", linestyle="--", alpha=0.5)
plt.ylim(0, 1)  # Efficiency ∈ [0,1]
plt.tight_layout()
plt.show()
