# t‑Digest Streaming Quantile Estimator

A high‑performance C++17 implementation of the t‑Digest algorithm for streaming quantile estimation. This project demonstrates advanced algorithmic thinking, statistical computing, multithreading, and performance optimization techniques relevant to quantitative finance, real‑time monitoring, and high‑frequency trading systems.

---

## 🎯 What It Does

- **Ingests** large streams of real numbers (unbounded, streaming data).  
- **Summarizes** into O(δ) centroids (adaptive buckets with uniform max‑weight).  
- **Answers** any percentile (quantile) query in O(log δ) time.  
- **Multithreaded** ingestion with batch buffering to maximize throughput.  
- **Benchmarks** both ingestion rate and query performance, plus accuracy statistics.

---

## 🚀 Performance Highlights

| Metric                         | Result                |
|--------------------------------|-----------------------|
| **Ingestion Throughput**       | ~11–16 M points/sec   |
| **Query Throughput**           | ~1.8 M queries/sec    |
| **Average Query Latency**      | ~0.55 µs per query    |
| **Centroids for 1 M points**   | ~500–600 (δ=500)      |
| **Mean Relative Error**        | ~0.013 %              |
| **95th‑Percentile Error**      | < 0.1 %               |
| **99th‑Percentile Error**      | ~0.3 %                |

---

## 📁 Repository Structure


---

## 🛠 Prerequisites

- **Compiler**: GCC 7+ or Clang 5+ with C++17 support  
- **CMake**: version 3.10 or higher  
- **Linux/macOS** (any POSIX environment)  
- **Make** (or Ninja)  

---

## 🏗 Build Instructions

```bash
    # Clone the repo:
    git clone https://github.com/yourusername/tdigest_project.git
    cd tdigest_project

    # Make the build script executable:
    chmod +x build.sh

    # Build in Release mode:
    ./build.sh

After running `./build.sh`, you will have these executables in `build/`:

- `data_generator`  
- `ground_truth`  
- `tdigest_main`  
- `benchmark`  

---

## 📊 Usage

### A) Run Everything

`run_all.sh` orchestrates the full workflow:

```bash
chmod +x run_all.sh
./run_all.sh

This script will:

- Generate 1 000 000 normal‑distribution samples (data/normal_data.txt).

- Compute exact quantiles (1%, 5%, 50%, 95%, 99%).

- Stream those samples through the multithreaded t‑Digest, reporting final centroids + key quantiles.

- Benchmark single‑threaded ingestion for δ = {50, 100, 200, 500}.

## B) Individual Steps

1. **Generate Data**  
   ```bash
   ./build/data_generator
2. **Ground‑Truth Quantiles**
   ```bash
    ./build/ground_truth data/normal_data.txt
3. **Streaming T-Digest**
    ```bash
    ./build/tdigest_main data/normal_data.txt 500 1000 5000 4
4. **Benchmark Ingestion**
    ```bash
    Benchmark Ingestion

# 🔧 **Tuning Parameters**
- δ (compression): larger δ → more centroids → higher accuracy.

- merge_threshold: how many raw centroids before calling merge() (≈ 2 × δ).

- batch_size: number of points to buffer before flushing to digest (1 000–5 000).

- threads: number of ingest threads (match to your CPU core count).