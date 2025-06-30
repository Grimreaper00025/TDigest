#pragma once
#include "tdigest.h"
#include <mutex>
#include <vector>
#include <atomic>

class ConcurrentTDigest {
private:
    TDigest td_;
    std::mutex mutex_;
    std::vector<double> buffer_;
    std::atomic<size_t> count_{0};
    size_t batch_size_;

public:
    ConcurrentTDigest(double compression = 100.0,
                      size_t merge_threshold = 200,
                      size_t batch_size = 1000);
    void add(double value);
    double quantile(double q);
    size_t size() const;
};