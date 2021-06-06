#include <emmintrin.h>
#include <smmintrin.h>

#include <iostream>
#include <ctime>

#include "generators.hpp"

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
    float* fa = generateFloatArray(fn);
    std::cout << "done\n";

    std::cout << "Generating int array...";
    int in = 30000;
    int* ia = generateIntArray(in);
    std::cout << "done\n";

    clock_t begin, end;
    double elapsed;

    // value-based search
    begin = clock();

    float m1 = max1(fa, fn);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "=== Looking for a maximum element in a list of " << fn << " floats ===\n";
    std::cout << "* Value-based: " << elapsed << " sec; max = " << m1 << "\n";

    // index-based search
    begin = clock();

    float m2 = max2(fa, fn);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "* Index-based: " << elapsed << " sec; max = " << m2 << "\n";

    // SSE
    begin = clock();

    float m3 = max3(fa, fn);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "* SSE: " << elapsed << " sec; max = " << m3 << "\n";

    // value-based on integers
    begin = clock();

    int m4 = max4(ia, in);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "=== Looking for a maximum element in a list of " << in << " integers ===\n";
    std::cout << "* Value-based: " << elapsed << " sec; max = " << m4 << "\n";

    // index-based on integers
    begin = clock();

    int m5 = max5(ia, in);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "* Index-based: " << elapsed << " sec; max = " << m5 << "\n";

    // SSE on integers
    begin = clock();

    int m6 = max6(ia, in);

    end = clock();
    elapsed = static_cast<double>(end - begin) / CLOCKS_PER_SEC;

    std::cout << "* SSE: " << elapsed << " sec; max = " << m6 << "\n";

    // prevent memory leaks
    delete fa;
    delete ia;

    return 0;
}
