#pragma once
#include <vector>
#include <cmath>

class TDigest {
private:
    struct Centroid {
        double mean;
        double weight;
        Centroid(double m, double w) : mean(m), weight(w) {}
        bool operator<(const Centroid& other) const {
            return mean < other.mean;
        }
    };

    std::vector<Centroid> centroids_;
    double total_weight_;
    double compression_;
    size_t merge_threshold_;

public:
    TDigest(double compression = 100.0, size_t merge_threshold = 200);
    void add(double value);
    void merge();
    double quantile(double q) const;
    size_t size() const { return centroids_.size(); }
    double total_weight() const { return total_weight_; }
};
