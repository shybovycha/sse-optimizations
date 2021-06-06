#include <xmmintrin.h>
#include <cstdio>

int main() {
    __m128 t0, t1, t2, t3;
    float* f0 = new float[4];
    float f1 = 0.0f;

    t0 = _mm_set_ps(4.0, 3.0, 2.0, 1.0);
    _mm_store_ps(f0, t0);
    std::printf("t0 = [%0.4f, %0.4f, %0.4f, %0.4f]\n", f0[3], f0[2], f0[1], f0[0]);

    _mm_store_ss(&f1, t0);
    std::printf("f1 = %0.4f\n", f1);

    t1 = _mm_set1_ps(3.5);
    f0 = new float[4];
    _mm_store_ps(f0, t1);
    std::printf("t1 = [%0.4f, %0.4f, %0.4f, %0.4f]\n", f0[3], f0[2], f0[1], f0[0]);

    t2 = _mm_set1_ps(2.7);
    f0 = new float[4];
    _mm_store_ss(f0, t2);
    std::printf("t2 = [%0.4f, %0.4f, %0.4f, %0.4f]\n", f0[3], f0[2], f0[1], f0[0]);

    t3 = _mm_setzero_ps();
    f0 = new float[4];
    _mm_store_ss(f0, t3);
    std::printf("t3 = [%0.4f, %0.4f, %0.4f, %0.4f]\n", f0[3], f0[2], f0[1], f0[0]);

    return 0;
}
