#include <memory>

std::unique_ptr<int[]> generateIntArray(const unsigned long long n) {
    std::unique_ptr<int[]> result{ new int[n] };

    for (auto i = 0; i < n; ++i) {
        result[i] = std::rand() % 30000;
    }

    return std::move(result);
}

std::unique_ptr<float[]> generateFloatArray(const unsigned long long n) {
    std::unique_ptr<float[]> result{ new float[n] };

    for (auto i = 0; i < n; ++i) {
        result[i] = std::rand();
    }

    return std::move(result);
}
