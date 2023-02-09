#include <xmmintrin.h>
#include <format>
#include <iostream>
#include <cstdio>

int main() {
    auto f1 = std::make_unique<float[]>(4);
    auto f2 = std::make_unique<float[]>(4);
    auto f3 = std::make_unique<float[]>(4);
    auto f4 = std::make_unique<float[]>(4);
    
    auto f0 = 0.0f;

    auto t0 = _mm_set_ps(4.0, 3.0, 2.0, 1.0);
    _mm_store_ps(f1.get(), t0);
    std::cout << std::format("t0 = [{:.4f}, {:.4f}, {:.4f}, {:.4f}]\n", f1[3], f1[2], f1[1], f1[0]);

    _mm_store_ss(&f0, t0); // copy the lower scalar (32 bits) from t0 to f1
    std::cout << std::format("f0 = {:.4f}\n", f0);

    auto t1 = _mm_set1_ps(3.5); // store 3.5 in all elements of t1
    _mm_store_ps(f2.get(), t1); // copy t1 to buffer f2
    std::cout << std::format("t1 = [{:.4f}, {:.4f}, {:.4f}, {:.4f}]\n", f2[3], f2[2], f2[1], f2[0]);

    _mm_store_ss(&f0, t1); // copy the lower scalar (32 bits) from t1 to f0
    std::cout << std::format("f0 = {:.4f}\n", f0);

    auto t2 = _mm_set1_ps(2.7); // store 2.7 in t2
    _mm_store_ps(f3.get(), t2); // copy t2 to buffer f3
    std::cout << std::format("t2 = [{:.4f}, {:.4f}, {:.4f}, {:.4f}]\n", f3[3], f3[2], f3[1], f3[0]);

    auto t3 = _mm_setzero_ps(); // zero-out t3
    _mm_store_ps(f4.get(), t3); // store t3 in f4
    std::cout << std::format("t3 = [{:.4f}, {:.4f}, {:.4f}, {:.4f}]\n", f4[3], f4[2], f4[1], f4[0]);

    return 0;
}
