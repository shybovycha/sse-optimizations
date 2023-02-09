#include <tmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>

#include <iostream>

#include <generators.hpp>
#include <benchmark.hpp>

/*
* Simple linear summation of an array
*/
float sum1(float* a, int n) {
    float res = 0.0;

    for (int i = 0; i < n; i++) {
        res += a[i];
    }

    return res;
}

/*
* 1/4-th of an array is being actually iterated; the SSE add every 4 elements
*/
float sum2(float* a, int n) {
    float res = 0.0;

    __m128* v4 = reinterpret_cast<__m128*>(a);
    __m128 vec_sum = _mm_setzero_ps();

    for (int i = 0; i < n / 4; i++) {
        vec_sum = _mm_add_ps(vec_sum, v4[i]);
    }

    vec_sum = _mm_hadd_ps(vec_sum, vec_sum);
    vec_sum = _mm_hadd_ps(vec_sum, vec_sum);

    _mm_store_ss(&res, vec_sum);

    return res;
}

unsigned long long sum3(int* a, int n) {
    unsigned long long res = 0;

    for (int i = 0; i < n; i++) {
        res += a[i];
    }

    return res;
}

unsigned long long sum4(int* a, int n) {
    unsigned long long res;

    __m128i* f4 = reinterpret_cast<__m128i*>(a);
    __m128i vec_sum = _mm_setzero_si128();

    for (int i = 0; i < n / 4; i++) {
        vec_sum = _mm_hadd_epi32(vec_sum, f4[i]);
    }

    vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);
    vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);

    res = _mm_cvtsi128_si32(vec_sum);

    return res;
}

int main() {
    int fn = 10000;
    int in = 30000;

    auto fa = generateFloatArray(fn);
    auto ia = generateIntArray(in);

    // value-based sum
    float s1 = 0.f; 
    
    auto elapsed1 = benchmark([&]() { s1 = sum1(fa.get(), fn); });

    std::cout << "=== Calculating sum of a list of " << fn << " floats ===\n";
    std::cout << "* Value-based: " << elapsed1 << " sec; sum = " << s1 << "\n";

    // SSE
    float s2 = 0.f;
    
    auto elapsed2 = benchmark([&]() { s2 = sum2(fa.get(), fn); });

    std::cout << "* SSE: " << elapsed2 << " sec; sum = " << s2 << "\n";

    // Value-based on integers
    unsigned long long s3 = 0;
    
    auto elapsed3 = benchmark([&]() { s3 = sum3(ia.get(), in); });

    std::cout << "=== Calculating sum of a list of " << in << " integers ===\n";
    std::cout << "* Value-based: " << elapsed3 << " sec; sum = " << s3 << "\n";

    // SSE on integers
    unsigned long long s4 = 0;
    
    auto elapsed4 = benchmark([&]() { s4 = sum4(ia.get(), in); });

    std::cout << "* SSE: " << elapsed4 << " sec; sum = " << s4 << "\n";

    return 0;
}
