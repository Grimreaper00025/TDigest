#include "../include/tdigest.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>

int main() {
    const size_t N = 5000000;
    std::mt19937_64 gen(std::random_device{}());
    std::normal_distribution<double> dist(0.0,1.0);
    std::vector<double> data(N);
    for (auto &x : data) x = dist(gen);

    for (double c : {50,100,200,500}) {
        TDigest td(c, 2*size_t(c));
        auto t0 = std::chrono::high_resolution_clock::now();
        for (auto &x : data) td.add(x);
        auto t1 = std::chrono::high_resolution_clock::now();
        double secs = std::chrono::duration<double>(t1 - t0).count();
        std::cout << "c="<<c
                  << "  rate="<< (N/secs)<<" pts/s"
                  << "  centroids="<<td.size()<<"\n";
    }
    return 0;
}