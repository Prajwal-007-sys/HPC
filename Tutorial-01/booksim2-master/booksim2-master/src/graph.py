import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


# ============================================================
# File paths
# ============================================================

mesh_uniform_file = Path("saturation_results/sampled_results_uniform.csv")
mesh_transpose_file = Path("saturation_results/sampled_results_transpose.csv")

torus_uniform_file = Path("torus88_results/sampled_results_uniform.csv")
torus_transpose_file = Path("torus88_results/sampled_results_transpose.csv")


# ============================================================
# Saturation latency values
# Replace these with your actual calculated values
# ============================================================

mesh_uniform_sat = 170.0
mesh_transpose_sat = 171.0

torus_uniform_sat = 170.0
torus_transpose_sat = 171.0


# ============================================================
# Read CSV files
# ============================================================

mesh_uniform = pd.read_csv(mesh_uniform_file)
mesh_transpose = pd.read_csv(mesh_transpose_file)

torus_uniform = pd.read_csv(torus_uniform_file)
torus_transpose = pd.read_csv(torus_transpose_file)


# ============================================================
# Function to create and save a graph
# ============================================================

def create_graph(data, saturation, title, filename):

    plt.figure(figsize=(8, 5))

    plt.plot(
        data["injection_rate"],
        data["network_latency"],
        marker="o",
        linewidth=1.5,
        label="Average Network Latency"
    )

    plt.axhline(
        y=saturation,
        linestyle="--",
        linewidth=1.5,
        label=f"Saturation Latency = {saturation:.2f}"
    )

    plt.xlabel("Injection Rate")
    plt.ylabel("Average Network Latency")
    plt.title(title)

    plt.legend()
    plt.grid(True)

    plt.tight_layout()

    plt.savefig(
        filename,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close()

    print(f"Saved: {filename}")


# ============================================================
# Create the four graphs
# ============================================================

create_graph(
    mesh_uniform,
    mesh_uniform_sat,
    "8×8 Mesh - Uniform Traffic",
    "mesh_uniform.png"
)

create_graph(
    mesh_transpose,
    mesh_transpose_sat,
    "8×8 Mesh - Transpose Traffic",
    "mesh_transpose.png"
)

create_graph(
    torus_uniform,
    torus_uniform_sat,
    "8×8 Torus - Uniform Traffic",
    "torus_uniform.png"
)

create_graph(
    torus_transpose,
    torus_transpose_sat,
    "8×8 Torus - Transpose Traffic",
    "torus_transpose.png"
)

print("\nAll four graphs created successfully.")