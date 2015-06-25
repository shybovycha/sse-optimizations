#include <emmintrin.h>
#include <smmintrin.h>
#include <stdio.h>
#include <ctime>
#include "float_array.h"
#include "int_array.h"

float max1(float *a, int n) {
	float res = a[0];

	for (int i = 1; i < n; i++) {
		if (a[i] > res)
			res = a[i];
	}

	return res;
}

float max2(float *a, int n) {
	int res = 0;

	for (int i = 1; i < n; i++) {
		if (a[i] > a[res])
			res = i;
	}

	return a[res];
}

float max3(float *a, int n) {
	float res;

	__m128 *f4 = (__m128*) a;
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

int max4(int *a, int n) {
	int res = a[0];

	for (int i = 1; i < n; i++) {
		if (a[i] > res)
			res = a[i];
	}

	return res;
}

int max5(int *a, int n) {
	int res = 0;

	for (int i = 1; i < n; i++) {
		if (a[i] > a[res])
			res = i;
	}

	return a[res];
}

int max6(int *a, int n) {
	int res;

	__m128i *f4 = (__m128i*) a;
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
	using namespace std;

	int fn = sizeof(fa) / sizeof(float);
	int in = sizeof(ia) / sizeof(int);

	clock_t begin, end;
	double elapsed;

	// value-based search
	begin = clock();

	float m1 = max1(fa, fn);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("Value-based: %0.8f (%0.8f)\n", elapsed, m1);

	// index-based search
	begin = clock();

	float m2 = max2(fa, fn);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("Index-based: %0.8f (%0.8f)\n", elapsed, m2);

	// SSE
	begin = clock();

	float m3 = max3(fa, fn);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("SSE: %0.8f (%0.8f)\n", elapsed, m3);

	// value-based on integers
	begin = clock();

	int m4 = max4(ia, in);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("Value-based on integers: %0.8f (%d)\n", elapsed, m4);

	// index-based on integers
	begin = clock();

	int m5 = max5(ia, in);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("Index-based on integers: %0.8f (%d)\n", elapsed, m5);

	// SSE4 on integers
	begin = clock();

	int m6 = max6(ia, in);

	end = clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;

	printf("SSE4 on integers: %0.8f (%d)\n", elapsed, m6);

	return 0;
}