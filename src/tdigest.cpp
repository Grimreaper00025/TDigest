#include "../include/tdigest.h"
#include <algorithm>
#include <limits>

TDigest::TDigest(double compression, size_t merge_threshold)
    : total_weight_(0.0),
      compression_(compression),
      merge_threshold_(merge_threshold)
{
    centroids_.reserve(static_cast<size_t>(compression_ * 2));
}

void TDigest::add(double value) {
    centroids_.emplace_back(value, 1.0);
    total_weight_ += 1.0;

    if (centroids_.size() > merge_threshold_) {
        merge();
    }
}

void TDigest::merge() {
    if (centroids_.size() <= 1) return;

    // 1) Sort all centroids by mean
    std::sort(centroids_.begin(), centroids_.end());

    // 2) Prepare output
    std::vector<Centroid> merged;
    merged.reserve(compression_ * 2);

    double W       = total_weight_;
    double maxW    = W / compression_;  // uniform bucket capacity
    double running = 0.0;

    for (auto &c : centroids_) {
        if (merged.empty()) {
            merged.push_back(c);
            running = c.weight;
        } else {
            // if we can fit into current bucket, merge
            if (merged.back().weight + c.weight <= maxW) {
                double w0 = merged.back().weight,
                       w1 = c.weight,
                       m0 = merged.back().mean,
                       m1 = c.mean,
                       newW = w0 + w1;
                merged.back().mean   = (m0*w0 + m1*w1) / newW;
                merged.back().weight = newW;
                running += c.weight;
            } else {
                // else start a new bucket
                merged.push_back(c);
                running = c.weight;
            }
        }
    }

    // 3) Swap in merged centroids
    centroids_.swap(merged);
    // total_weight_ remains correct
}

double TDigest::quantile(double q) const {
    if (centroids_.empty()) return std::numeric_limits<double>::quiet_NaN();
    if (q <= 0.0) return centroids_.front().mean;
    if (q >= 1.0) return centroids_.back().mean;

    double target = q * total_weight_;
    double cumulative = 0.0;

    // fast-path: all weights==1
    bool all_unit = true;
    for (auto &c : centroids_) {
        if (c.weight != 1.0) { all_unit = false; break; }
    }
    if (all_unit) {
        size_t idx = std::min<size_t>(centroids_.size() - 1,
            static_cast<size_t>(q * centroids_.size()));
        return centroids_[idx].mean;
    }

    for (size_t i = 0; i < centroids_.size(); ++i) {
        double w = centroids_[i].weight;
        if (cumulative + w >= target) {
            // interpolate
            if (i + 1 < centroids_.size()) {
                double m0 = centroids_[i].mean, m1 = centroids_[i + 1].mean;
                double left = target - cumulative;
                double frac = left / w - 0.5;
                return m0 + frac * (m1 - m0);
            }
            return centroids_[i].mean;
        }
        cumulative += w;
    }
    return centroids_.back().mean;
}
