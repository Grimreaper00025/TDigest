#include "../include/concurrent_tdigest.h"
#include "../include/tdigest.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <thread>
#include <random>
#include <algorithm>
#include <cmath>

// Ground‐truth utility
static double true_quantile(const std::vector<double>& A, double q) {
    if (A.empty()) return 0.0;
    if (q <= 0.0) return A.front();
    if (q >= 1.0) return A.back();
    double idx = q * (A.size() - 1);
    size_t lo = (size_t)std::floor(idx), hi = (size_t)std::ceil(idx);
    double w = idx - lo;
    return A[lo] * (1 - w) + A[hi] * w;
}

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cerr << "Usage: " << argv[0]
                  << " <data_file> <compression> <merge_thr> <batch_size> <threads>\n";
        return 1;
    }

    // 1) Parse arguments
    std::string file       = argv[1];
    double    compression = std::stod(argv[2]);
    size_t    merge_thr   = std::stoul(argv[3]);
    size_t    batch_size  = std::stoul(argv[4]);
    unsigned  threads     = std::stoul(argv[5]);

    // 2) Multithreaded ingestion
    ConcurrentTDigest digest(compression, merge_thr, batch_size);
    std::vector<std::thread> pros;
    for (unsigned t = 0; t < threads; ++t) {
        pros.emplace_back([&,t]() {
            std::ifstream in(file);
            double v;
            size_t cnt = 0;
            while (in >> v) {
                if ((cnt++ + t) % threads == 0) {
                    digest.add(v);
                }
            }
        });
    }
    for (auto &th : pros) th.join();

    // Final merge before querying
    digest.quantile(0.5);

    // 3) Report streaming results
    std::cout << "\n--- Streaming t‑Digest Results ---\n";
    std::cout << "Final centroids: " << digest.size() << "\n";
    for (double q : {0.01, 0.05, 0.5, 0.95, 0.99}) {
        std::cout << "Q" << q << ": " << digest.quantile(q) << "\n";
    }

    // 4) Load data for single-thread benchmarks & accuracy
    std::vector<double> data;
    {
        std::ifstream in(file);
        double v;
        while (in >> v) data.push_back(v);
    }

    // 5) Single-threaded TDigest build
    TDigest td(compression, merge_thr);
    for (double x : data) td.add(x);
    td.merge();

    // 6) Prepare random quantile queries
    const int QN = 10000000;
    std::vector<double> qs(QN);
    std::mt19937_64 gen(std::random_device{}());
    std::uniform_real_distribution<double> uq(0.0, 1.0);
    for (int i = 0; i < QN; ++i) qs[i] = uq(gen);

    // 7) Benchmark bulk queries
    using clock = std::chrono::high_resolution_clock;
    auto q_start = clock::now();
    volatile double sink = 0.0;
    for (double q : qs) sink += td.quantile(q);
    auto q_end = clock::now();
    double q_secs = std::chrono::duration<double>(q_end - q_start).count();

    std::cout << "\n--- Query Benchmark ---\n";
    std::cout << "Queries: " << QN
              << "  Rate: " << (QN / q_secs) << " q/s"
              << "  Avg Latency: " << (q_secs / QN) * 1e6 << " µs\n";

    // 8) Accuracy statistics
    std::sort(data.begin(), data.end());
    double total_err = 0.0;
    std::vector<double> errors;
    errors.reserve(QN);

    for (double q : qs) {
        double truth    = true_quantile(data, q);
        double estimate = td.quantile(q);
        double err      = std::abs(estimate - truth) / std::abs(truth);
        total_err       += err;
        errors.push_back(err);
    }

    std::sort(errors.begin(), errors.end());
    double p95 = errors[size_t(0.95 * QN)];
    double p99 = errors[size_t(0.99 * QN)];

    std::cout << "\n--- Accuracy Statistics ---\n";
    std::cout << "Mean Relative Error: " << (total_err / QN) * 100 << " %\n";
    std::cout << "95th‑Percentile Error: " << p95 * 100 << " %\n";
    std::cout << "99th‑Percentile Error: " << p99 * 100 << " %\n";

    // 9) Space Complexity (memory footprint)
    // Each centroid stores two doubles
    size_t centroid_count = digest.size();
    size_t mem_bytes = centroid_count * 2 * sizeof(double);
    std::cout << "\n--- Space Complexity ---\n";
    std::cout << "Centroids: " << centroid_count << "\n";
    std::cout << "Memory footprint: " 
              << (mem_bytes / 1024.0) << " KB (" 
              << centroid_count << " × " 
              << (2 * sizeof(double)) << " bytes)\n";

    return 0;
}
