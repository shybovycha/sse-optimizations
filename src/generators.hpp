#include <cstdlib>

int* generateIntArray(const unsigned long long n) {
    int* result = new int[n];

    for (auto i = 0; i < n; ++i) {
        result[i] = std::rand() % 30000;
    }

    return result;
}

float* generateFloatArray(const unsigned long long n) {
    float* result = new float[n];

    for (auto i = 0; i < n; ++i) {
        result[i] = std::rand();
    }

    return result;
}
