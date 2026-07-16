#include <iostream>
#include <vector>
#include <cstdint>
#include <chrono>
#include <new>

double MeasureL1CacheLatency() {
    constexpr size_t CacheLineSize = std::hardware_destructive_interference_size;
    constexpr size_t EvictCacheLineSize = CacheLineSize * (1 << 13); // 512 kb
    constexpr size_t TRIALS = 20000;

    std::vector<uint8_t> CacheLineData(CacheLineSize);
    std::vector<uint8_t> CacheLineEvictData(EvictCacheLineSize);

    volatile uint64_t sink = 0;
    double floodTime = 0.0;

    const auto startTrialing = std::chrono::steady_clock::now();

    for (size_t trials{}; trials < TRIALS; trials++) {
        auto floodStart = std::chrono::steady_clock::now();
        for (size_t i{}; i < CacheLineEvictData.size(); i += CacheLineSize) {
            sink += CacheLineEvictData[i];
        }
        auto floodEnd = std::chrono::steady_clock::now();

        const auto iterationFloodTime = std::chrono::duration_cast<std::chrono::nanoseconds>(floodEnd - floodStart).count();
        floodTime += static_cast<double>(iterationFloodTime);

        sink += CacheLineData[0];
    }

    const auto endTrialing = std::chrono::steady_clock::now();
    const double totalTrialTime = static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(endTrialing - startTrialing).count());

    const double totalReadLatency = totalTrialTime - floodTime;
    return totalReadLatency / TRIALS;
}

int main() {
    std::cout << MeasureL1CacheLatency();
    return 0;
}
