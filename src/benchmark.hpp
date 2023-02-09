#include <chrono>
// #include <iostream>
// #include <format>
#include <functional>
// #include <string>

std::chrono::duration<double, std::milli> benchmark(std::function<void()> fn) {
    auto start = std::chrono::steady_clock::now();
    
    fn();
    
    auto end = std::chrono::steady_clock::now();
    
    return end - start;

    // std::cout << std::format("[{}] {} ms\n", label, elapsed_millis);
}
