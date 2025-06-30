#include <iostream>
#include <fstream>
#include <random>

int main() {
    const size_t N = 1000000;
    std::mt19937_64 gen(std::random_device{}());
    std::normal_distribution<double> dist(100.0, 15.0);

    std::ofstream out("data/normal_data.txt");
    for (size_t i = 0; i < N; ++i) {
        out << dist(gen) << "\n";
    }
    std::cout << "Generated data/normal_data.txt (" << N << " samples)\n";
    return 0;
}