#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

double true_quantile(const std::vector<double>& A, double q) {
    if (A.empty()) return 0.0;
    if (q <= 0.0) return A.front();
    if (q >= 1.0) return A.back();
    double idx = q * (A.size() - 1);
    size_t lo = (size_t)std::floor(idx), hi = (size_t)std::ceil(idx);
    double w = idx - lo;
    return A[lo] * (1 - w) + A[hi] * w;
}

int main(int argc, char** argv) {
    if (argc != 2) { std::cerr<<"Usage: "<<argv[0]<<" <file>\n"; return 1; }
    std::ifstream in(argv[1]);
    std::vector<double> A;
    double x;
    while (in >> x) A.push_back(x);
    std::sort(A.begin(), A.end());
    for (double q : {0.01,0.05,0.5,0.95,0.99}) {
        std::cout<<"Q"<<q<<": "<<true_quantile(A,q)<<"\n";
    }
    return 0;
}
