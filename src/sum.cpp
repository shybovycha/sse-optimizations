#include <tmmintrin.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#include <stdio.h>
#include <ctime>
#include "float_array.h"
#include "int_array.h"

float sum1(float *a, int n) {
	float res = 0.0;

	for (int i = 0; i < n; i++) {
		res += a[i];
	}

	return res;
}

float sum2(float *a, int n) {
	float res = 0.0;

	__m128 *v4 = (__m128*) a;
	__m128 vec_sum = _mm_setzero_ps();

	for (int i = 0; i < n / 4; i++) {
		vec_sum = _mm_add_ps(vec_sum, v4[i]);
	}

	vec_sum = _mm_hadd_ps(vec_sum, vec_sum);
	vec_sum = _mm_hadd_ps(vec_sum, vec_sum);

	_mm_store_ss(&res, vec_sum);

	return res;
}

unsigned long long sum3(int *a, int n) {
	unsigned long long res = 0;

	for (int i = 0; i < n; i++) {
		res += a[i];
	}

	return res;
}

unsigned long long sum4(int *a, int n) {
	unsigned long long res;

	__m128i *f4 = (__m128i*) a;
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
	using namespace std;

	int fn = sizeof(fa) / sizeof(float);
	int in = sizeof(ia) / sizeof(float);

	clock_t begin, end;
	double elapsed;

	// value-based sum
	begin = clock();

	float s1 = sum1(fa, fn);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("=== Calculating sum of a list of %d floats ===\n", fn);
	printf("* Value-based: %0.8f sec; sum = %0.8f\n", elapsed, s1);

	// SSE
	begin = clock();

	float s2 = sum2(fa, fn);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("* SSE: %0.8f sec; sum = %0.8f\n", elapsed, s2);

	// Value-based on integers
	begin = clock();

	unsigned long long s3 = sum3(ia, in);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("=== Calculating sum of a list of %d integers ===\n", in);
	printf("* Value-based: %0.8f sec; sum = %lld\n", elapsed, s3);

	// SSE on integers
	begin = clock();

	unsigned long long s4 = sum4(ia, in);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("* SSE: %0.8f sec; sum = %lld\n", elapsed, s4);

	return 0;
}
