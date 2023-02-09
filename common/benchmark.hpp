#include <chrono>
#include <functional>

std::chrono::duration<double, std::milli> benchmark(std::function<void()> fn) {
    auto start = std::chrono::steady_clock::now();
    
    fn();
    
    auto end = std::chrono::steady_clock::now();
    
    return end - start;
}
