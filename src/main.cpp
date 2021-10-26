///
/// main.cpp
/// @author  andrew.
///

#include "logger_af.h"

#include <thread>
#include <vector>
#include <algorithm>

#include <random>
#include <iostream>


template<Logger_AF::Level LEVEL>
void
many_outputs(std::size_t thread_num, std::size_t max_iterations, double sleep_time, double range) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(sleep_time - range, sleep_time + range);

    auto time_to_sleep_offset = dist(mt);
    sleep_time += time_to_sleep_offset;

    dist = std::uniform_real_distribution<double>{sleep_time - range, sleep_time + range};

    for (
            decltype(max_iterations) count = 0;
            count < max_iterations;
            count++) {
        Logger_AF::Logger::put<LEVEL>("Hello from thread {}, this is iteration {}", thread_num, count);
        auto time_to_sleep = std::chrono::milliseconds(static_cast<std::size_t>(dist(mt)));
        std::this_thread::sleep_for(time_to_sleep);
    }
}


int main() {
    decltype(std::thread::hardware_concurrency()) min_threads = 2;
    const auto processor_count = (std::max(std::thread::hardware_concurrency(),
                                           min_threads) - 1);

    std::vector<std::thread> threads{};
    threads.reserve(processor_count);

    double time_sleep = 500.0;
    double time_range = 400.0;


    std::size_t iters_requested = 15;
    threads.emplace_back(many_outputs<Logger_AF::Level::FATAL>, 0, iters_requested, time_sleep, time_range);
    threads.emplace_back(many_outputs<Logger_AF::Level::ERROR>, 1, iters_requested, time_sleep, time_range);
    threads.emplace_back(many_outputs<Logger_AF::Level::WARN>, 2, iters_requested, time_sleep, time_range);
    threads.emplace_back(many_outputs<Logger_AF::Level::INFO>, 3, iters_requested, time_sleep, time_range);
    threads.emplace_back(many_outputs<Logger_AF::Level::DEBUG>, 4, iters_requested, time_sleep, time_range);
    threads.emplace_back(many_outputs<Logger_AF::Level::TRACE>, 5, iters_requested, time_sleep, time_range);

    for (std::size_t i = threads.size(); i < processor_count; i++) {
        threads.emplace_back(many_outputs<Logger_AF::Level::TRACE>, i, iters_requested, time_sleep, time_range);
    }

    for (auto &th: threads) {
        th.join();
    }
}

