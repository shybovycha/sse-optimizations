# SSE examples

**Note:** please, refer to [this blog post](https://shybovycha.github.io/2017/02/21/speeding-up-algorithms-with-sse.html) for more details and explanations.

## Building

Use CMake: `cmake -B build -H . && cmake --build build`.
You'll need a modern C++ compiler and a CPU with SSE4/AVX features support.

## Running

Executables are put in the `build/` directory.
Run `./build/1-basics/1-basics`, `./build/3-max/3-max` or `./build/2-sum/2-sum` to start the corresponding sample.

## Samples

The samples showcase the use of SSE intrinsics to speed up some trivial operations such as
finding maximum element in a list of floats or summation of an array of numbers.
