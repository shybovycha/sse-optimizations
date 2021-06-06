#include <tmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>

#include <iostream>
#include <ctime>

#include "generators.hpp"

float sum1(float* a, int n) {
    float res = 0.0;

    for (int i = 0; i < n; i++) {
        res += a[i];
    }

    return res;
}

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

    float* fa = generateFloatArray(fn);
    int* ia = generateIntArray(in);

    clock_t begin, end;
    double elapsed;

    // value-based sum
    begin = clock();

    float s1 = sum1(fa, fn);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "=== Calculating sum of a list of " << fn << " floats ===\n";
    std::cout << "* Value-based: " << elapsed << " sec; sum = " << s1 << "\n";

    // SSE
    begin = clock();

    float s2 = sum2(fa, fn);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "* SSE: " << elapsed << " sec; sum = " << s2 << "\n";

    // Value-based on integers
    begin = clock();

    unsigned long long s3 = sum3(ia, in);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "=== Calculating sum of a list of " << in << " integers ===\n";
    std::cout << "* Value-based: " << elapsed << " sec; sum = " << s3 << "\n";

    // SSE on integers
    begin = clock();

    unsigned long long s4 = sum4(ia, in);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "* SSE: " << elapsed << " sec; sum = " << s4 << "\n";

    // prevent memory leaks
    delete fa;
    delete ia;

    return 0;
}
