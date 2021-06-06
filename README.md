# SSE examples

**Note:** please, refer to [this blog post](http://shybovycha.github.io/2017/02/21/speeding-up-algorithms-with-sse.html) for details and explanations.

## Building

Use CMake: `cmake -B build -H . && cmake --build build`.
You'll need a modern C++ compiler and a processor with SSE4 support.

## Running

Executables are put in the `build/` directory.
Run `./build/sse-basics`, `./build/sse-max` or `./build/sse-sum` to start the corresponding sample.
