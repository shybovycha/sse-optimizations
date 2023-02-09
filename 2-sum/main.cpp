#include <tmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>

#include <iostream>

#include <generators.hpp>
#include <benchmark.hpp>

/*
* Simple linear summation of an array
*/
float float_sum_linear(float* a, int n) {
    float res = 0.0;

    for (int i = 0; i < n; i++) {
        res += a[i];
    }

    return res;
}

/*
* 1/4-th of an array is being actually iterated; the SSE add every 4 elements
*/
float float_sum_sse_128(float* a, int n) {
    float res = 0.0;

    __m128* v4 = reinterpret_cast<__m128*>(a);
    __m128 vec_sum = _mm_setzero_ps();

    for (int i = 0; i < n / 4; i++) {
        vec_sum = _mm_add_ps(vec_sum, v4[i]);
    }

    vec_sum = _mm_hadd_ps(vec_sum, vec_sum);
    vec_sum = _mm_hadd_ps(vec_sum, vec_sum);

    auto vec = std::make_unique<float[]>(4);
    _mm_store_ps(vec.get(), vec_sum);
    std::cout << std::format("SSE vec: [ {:.4f}, {:.4f}, {:.4f}, {:.4f} ]\n", vec[3], vec[2], vec[1], vec[0]);

    res = _mm_cvtss_f32(vec_sum);

    return res;
}

float float_sum_sse_256(float* a, int n) {
    float res = 0.0;

    __m256* v8 = reinterpret_cast<__m256*>(a);
    __m256 vec_sum = _mm256_setzero_ps();

    for (int i = 0; i < n / 8; i++) {
        vec_sum = _mm256_add_ps(vec_sum, v8[i]);
    }

    vec_sum = _mm256_hadd_ps(vec_sum, vec_sum);
    vec_sum = _mm256_hadd_ps(vec_sum, vec_sum);
    
    __m256i indexes = _mm256_setr_epi32(0, 4, 2, 6, 1, 5, 2, 7);
    vec_sum = _mm256_permutevar8x32_ps(vec_sum, indexes);

    vec_sum = _mm256_hadd_ps(vec_sum, vec_sum);

    res = _mm256_cvtss_f32(vec_sum);

    return res;
}

unsigned long long int_sum_linear(int* a, int n) {
    unsigned long long res = 0;

    for (int i = 0; i < n; i++) {
        res += a[i];
    }

    return res;
}

unsigned long long int_sum_sse_128(int* a, int n) {
    unsigned long long res;

    __m128i* i4 = reinterpret_cast<__m128i*>(a);
    __m128i vec_sum = _mm_setzero_si128();

    for (int i = 0; i < n / 4; i++) {
        vec_sum = _mm_add_epi32(vec_sum, i4[i]);
    }

    /*auto vec = std::make_unique<int[]>(4);
    _mm_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("SSE vec 0: [ {}, {}, {}, {} ]\n", vec[3], vec[2], vec[1], vec[0]);

    vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);

    _mm_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("SSE vec 1: [ {}, {}, {}, {} ]\n", vec[3], vec[2], vec[1], vec[0]);

    vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);

    _mm_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("SSE vec 2: [ {}, {}, {}, {} ]\n", vec[3], vec[2], vec[1], vec[0]);*/

    vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);
    vec_sum = _mm_hadd_epi32(vec_sum, vec_sum);

    res = _mm_cvtsi128_si32(vec_sum);

    return res;
}

/*
* it is trickier with 8 numbers, but here it goes:
* 
* vec0 = [ 8, 7, 6, 5, 4, 3, 2, 1 ]
* 
* vec1 = hadd_epi32(vec0, vec0) = [ 15, 11, 15, 11, 7, 3, 7, 3 ]
* 
* vec2 = hadd_epi32(vec1, vec1) = [ 26, 26, 26, 26, 10, 10, 10, 10 ]
* 
* vec3 = hadd_epi32(vec2, vec2) = [ 52, 52, 52, 52, 20, 20, 20, 20 ]
* 
* which is wrong - we need to shuffle the numbers in lower and higher halves and add them pair-wise again
* to get the right result
* 
* indexes = [ 0, 4, 2, 6, 1, 5, 2, 7 ]
* vec2 = _mm256_permutevar8x32_epi32(vec2, indexes) = [ 26, 10, 26, 10, 26, 10, 26, 10 ]
* 
* vec3 = hadd(vec2, vec2) = [ 36, 36, 36, 36, 36, 36, 36, 36 ]
*/
unsigned long long int_sum_sse_256(int* a, int n) {
    unsigned long long res;

    __m256i* i8 = reinterpret_cast<__m256i*>(a);
    __m256i vec_sum = _mm256_setzero_si256();

    for (int i = 0; i < n / 8; i++) {
        vec_sum = _mm256_add_epi32(vec_sum, i8[i]);
    }

    /*auto vec = std::make_unique<int[]>(8);
    _mm256_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("AVX vec 0: [ {}, {}, {}, {}, {}, {}, {}, {} ]\n", vec[7], vec[6], vec[5], vec[4], vec[3], vec[2], vec[1], vec[0]);

    vec_sum = _mm256_hadd_epi32(vec_sum, vec_sum);

    _mm256_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("AVX vec 1: [ {}, {}, {}, {}, {}, {}, {}, {} ]\n", vec[7], vec[6], vec[5], vec[4], vec[3], vec[2], vec[1], vec[0]);

    vec_sum = _mm256_hadd_epi32(vec_sum, vec_sum);

    _mm256_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("AVX vec 2: [ {}, {}, {}, {}, {}, {}, {}, {} ]\n", vec[7], vec[6], vec[5], vec[4], vec[3], vec[2], vec[1], vec[0]);

    // move the numbers around to interleave the high and low 4 numbers with each other:
    // we want [a, b, a, b, a, b, a, b] for [a, a, a, a, b, b, b, b]
    __m256i indexes = _mm256_setr_epi32(0, 4, 2, 6, 1, 5, 2, 7);
    vec_sum = _mm256_permutevar8x32_epi32(vec_sum, indexes);

    _mm256_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("AVX vec permutated: [ {}, {}, {}, {}, {}, {}, {}, {} ]\n", vec[7], vec[6], vec[5], vec[4], vec[3], vec[2], vec[1], vec[0]);

    vec_sum = _mm256_hadd_epi32(vec_sum, vec_sum);

    _mm256_storeu_epi32(vec.get(), vec_sum);
    std::cout << std::format("AVX vec 3: [ {}, {}, {}, {}, {}, {}, {}, {} ]\n", vec[7], vec[6], vec[5], vec[4], vec[3], vec[2], vec[1], vec[0]);*/

    vec_sum = _mm256_hadd_epi32(vec_sum, vec_sum);

    vec_sum = _mm256_hadd_epi32(vec_sum, vec_sum);

    __m256i indexes = _mm256_setr_epi32(0, 4, 2, 6, 1, 5, 2, 7);
    vec_sum = _mm256_permutevar8x32_epi32(vec_sum, indexes);

    vec_sum = _mm256_hadd_epi32(vec_sum, vec_sum);

    res = _mm256_cvtsi256_si32(vec_sum);

    return res;
}

int main() {
    int fn = 10000;
    int in = 30000;

    auto fa = generateFloatArray(fn);
    auto ia = generateIntArray(in);

    // value-based sum
    float float_sum1 = 0.f; 
    
    auto elapsed1 = benchmark([&]() { float_sum1 = float_sum_linear(fa.get(), fn); });

    std::cout << "=== Calculating sum of a list of " << fn << " floats ===\n";
    std::cout << "* Value-based: " << elapsed1 << " sec; sum = " << float_sum1 << "\n";

    // SSE
    float float_sum2 = 0.f;
    
    auto elapsed2 = benchmark([&]() { float_sum2 = float_sum_sse_128(fa.get(), fn); });

    std::cout << "* SSE: " << elapsed2 << " sec; sum = " << float_sum2 << "\n";

    // AVX
    float float_sum3 = 0.f;

    auto elapsed3 = benchmark([&]() { float_sum3 = float_sum_sse_256(fa.get(), fn); });

    std::cout << "* AVX: " << elapsed3 << " sec; sum = " << float_sum3 << "\n";

    // Value-based on integers
    unsigned long long int_sum1 = 0;
    
    auto elapsed4 = benchmark([&]() { int_sum1 = int_sum_linear(ia.get(), in); });

    std::cout << "=== Calculating sum of a list of " << in << " integers ===\n";
    std::cout << "* Value-based: " << elapsed4 << " sec; sum = " << int_sum1 << "\n";

    // SSE on integers
    unsigned long long int_sum2 = 0;
    
    auto elapsed5 = benchmark([&]() { int_sum2 = int_sum_sse_128(ia.get(), in); });

    std::cout << "* SSE: " << elapsed5 << " sec; sum = " << int_sum2 << "\n";

    // SSE on integers
    unsigned long long int_sum3 = 0;

    auto elapsed6 = benchmark([&]() { int_sum3 = int_sum_sse_256(ia.get(), in); });

    std::cout << "* AVX: " << elapsed6 << " sec; sum = " << int_sum3 << "\n";

    return 0;
}
