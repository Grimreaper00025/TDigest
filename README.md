# t‑Digest Streaming Quantile Estimator

A high‑performance C++17 implementation of the t‑Digest algorithm for streaming quantile estimation. Designed for real‑time data processing, this project demonstrates advanced algorithmic techniques, statistical computing, multithreading, and performance optimization suitable for quantitative finance, monitoring systems, and high‑frequency data applications.

---

## Overview

This repository provides:

* **Static library** (`tdigest_lib`): Core t‑Digest data structure with concurrent wrapper.
* **Executables** for data generation, ground‑truth quantile computation, streaming ingestion, and benchmarking.
* **Build scripts** (`build.sh`, `run_all.sh`) to automate compilation and full workflow execution.

## Features

* **Streaming ingestion** of unbounded real‑valued data streams.
* **Adaptive summarization** into O(δ) centroids (max‑weight adaptive buckets).
* **Quantile queries** answered in O(log δ) time.
* **Multithreaded ingestion** with batch buffering for maximum throughput.
* **Accuracy reporting** against exact quantiles (ground truth).
* **Ingestion and query benchmarking** with performance metrics.

## Repository Structure

```text
├── build.sh            # Build automation script
├── run_all.sh          # Full workflow script
├── CMakeLists.txt      # CMake project definition
├── include/            # Public headers
│   ├── tdigest.h
│   └── concurrent_tdigest.h
└── src/                # Source files
    ├── data_generator.cpp
    ├── ground_truth.cpp
    ├── benchmark.cpp
    ├── tdigest.cpp
    ├── concurrent_tdigest.cpp
    └── main.cpp        # Streaming application entry point
```

## Prerequisites

* **Compiler**: GCC 7+ or Clang 5+ (C++17 support)
* **CMake**: version 3.10 or higher
* **POSIX environment** (Linux, macOS)
* **Make** or **Ninja** build system

## Build Instructions

1. Clone the repository:

   ```bash
   git clone https://github.com/Grimreaper00025/TDigest.git
   cd TDigest
   ```
2. Make the build script executable and run it:

   ```bash
   chmod +x build.sh
   ./build.sh
   ```

This produces the following executables in `build/`:

* `data_generator`
* `ground_truth`
* `tdigest_main`
* `benchmark`

## Usage

### Full Workflow

Make `run_all.sh` executable and execute it to run the complete pipeline end‑to‑end:

```bash
chmod +x run_all.sh
./run_all.sh
```

This sequence will:

1. **Generate data** (`1_000_000` samples) into `data/normal_data.txt`.
2. **Compute exact quantiles** (1%, 5%, 50%, 95%, 99%).
3. **Stream ingestion** via multithreaded t‑Digest, reporting final centroids and quantiles.
4. **Benchmark single‑threaded ingestion** for various compression factors.

### Individual Steps

* **Generate Data**:

  ```bash
  ./build/data_generator
  ```

* **Ground‑Truth Quantiles**:

  ```bash
  ./build/ground_truth data/normal_data.txt
  ```

* **Streaming t‑Digest**:

  ```bash
  ./build/tdigest_main data/normal_data.txt 500 1000 5000 4
  ```

* **Benchmark Ingestion**:

  ```bash
  ./build/benchmark
  ```

## Benchmark Results

**1) Data Generation**

```
Generated data/normal_data.txt (10000000 samples)
```

**2) Ground‑Truth Quantiles**

```
Q0.01: 65.0716
Q0.05: 75.3137
Q0.5:  99.9821
Q0.95:124.668
Q0.99:134.927
```

**3) Streaming t‑Digest**

```
--- Streaming t‑Digest Results ---
Final centroids: 635
Q0.01: 65.0685
Q0.05: 75.3086
Q0.5:  99.9813
Q0.95:124.67
Q0.99:134.91

--- Query Benchmark ---
Queries: 10000000  Rate: 1.90964e+06 q/s  Avg Latency: 0.52366 µs

--- Accuracy Statistics ---
Mean Relative Error:    0.0127561 %
95th‑Percentile Error:  0.00932085 %
99th‑Percentile Error:  0.0926393 %

--- Space Complexity ---
Centroids: 635
Memory footprint: 9.92188 KB (635 × 16 bytes)
```

**4) Single‑Thread Benchmark**

```
c=50   rate=8.4475e+06 pts/s  centroids=82
c=100  rate=7.45068e+06 pts/s centroids=134
c=200  rate=7.14381e+06 pts/s centroids=388
c=500  rate=6.43611e+06 pts/s centroids=696
```

## Tuning Parameters

* **δ (compression)**: Controls max centroids. Higher δ → more centroids → higher accuracy.
* **merge\_threshold**: Flush threshold for raw centroids (≈ 2 × δ).
* **batch\_size**: Number of points buffered before merging (1 000–5 000).
* **threads**: Number of ingestion threads (ideally match CPU cores).

