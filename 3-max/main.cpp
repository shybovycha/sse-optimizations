#include <emmintrin.h>
#include <smmintrin.h>

#include <iostream>

#include <generators.hpp>
#include <benchmark.hpp>

float max1(float* a, int n) {
    float res = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] > res) {
            res = a[i];
        }
    }

    return res;
}

float max2(float* a, int n) {
    int res = 0;

    for (int i = 1; i < n; i++) {
        if (a[i] > a[res]) {
            res = i;
        }
    }

    return a[res];
}

float max3(float* a, int n) {
    float res;

    __m128* f4 = reinterpret_cast<__m128*>(a);
    __m128 maxval = _mm_setzero_ps();

    for (int i = 0; i < n / 4; i++) {
        maxval = _mm_max_ps(maxval, f4[i]);
    }

    for (int i = 0; i < 3; i++) {
        maxval = _mm_max_ps(maxval, _mm_shuffle_ps(maxval, maxval, 0x93));
    }

    _mm_store_ss(&res, maxval);

    return res;
}

int max4(int* a, int n) {
    int res = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] > res) {
            res = a[i];
        }
    }

    return res;
}

int max5(int* a, int n) {
    int res = 0;

    for (int i = 1; i < n; i++) {
        if (a[i] > a[res]) {
            res = i;
        }
    }

    return a[res];
}

int max6(int* a, int n) {
    int res;

    __m128i* f4 = reinterpret_cast<__m128i*>(a);
    __m128i maxval = _mm_setzero_si128();

    for (int i = 0; i < n / 4; i++) {
        maxval = _mm_max_epi32(maxval, f4[i]);
    }

    for (int i = 0; i < 3; i++) {
        maxval = _mm_max_epi32(maxval, _mm_shuffle_epi32(maxval, 0x93));
    }

    res = _mm_cvtsi128_si32(maxval);

    return res;
}

int main() {
    std::cout << "Generating float array...";
    
    int fn = 10000;
    auto fa = generateFloatArray(fn);

    std::cout << "done\n";

    std::cout << "Generating int array...";

    int in = 30000;
    auto ia = generateIntArray(in);

    std::cout << "done\n";

    // value-based search
    float m1 = 0.f;

    std::cout << "=== Looking for a maximum element in a list of " << fn << " floats ===\n";

    auto elapsed1 = benchmark([&]() { m1 = max1(fa.get(), fn); });

    std::cout << "* Value-based: " << elapsed1 << " ms; max = " << m1 << "\n";

    // index-based search
    float m2 = 0.f;

    auto elapsed2 = benchmark([&]() { m2 = max2(fa.get(), fn); });

    std::cout << "* Index-based: " << elapsed2 << " ms; max = " << m2 << "\n";

    // SSE
    float m3 = 0.f;

    auto elapsed3 = benchmark([&]() { m3 = max3(fa.get(), fn); });

    std::cout << "* SSE: " << elapsed3 << " ms; max = " << m3 << "\n";

    // value-based on integers
    int m4 = 0;
    
    std::cout << "=== Looking for a maximum element in a list of " << in << " integers ===\n";

    auto elapsed4 = benchmark([&]() { m4 = max4(ia.get(), in); });

    std::cout << "* Value-based: " << elapsed4 << " ms; max = " << m4 << "\n";

    // index-based on integers
    int m5 = 0;

    auto elapsed5 = benchmark([&]() { m5 = max5(ia.get(), in); });

    std::cout << "* Index-based: " << elapsed5 << " ms; max = " << m5 << "\n";

    // SSE on integers
    int m6 = 0;

    auto elapsed6 = benchmark([&]() { m6 = max6(ia.get(), in); });

    std::cout << "* SSE: " << elapsed6 << " ms; max = " << m6 << "\n";

    return 0;
}
