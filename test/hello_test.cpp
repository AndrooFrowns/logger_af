///
/// hello_test.cpp
/// @author  andrew.
///

#include <gtest/gtest.h>

#include "../src/logger_af.h"

#include <thread>
#include <vector>


TEST(HelloTest, BasicAssertions) {
    // test of Google test because I haven't used it before.  Not sure exactly how to test cout and cerr though
    EXPECT_STRNE("HELLO", "WORLD");

    EXPECT_EQ(4 * 7, 28);

    std::size_t i = 0;


    auto threads_available = 7;

    std::vector<std::thread> threads{};
    threads.reserve(threads_available);

    for (std::size_t i; i < threads_available; i++) {
        threads.emplace_back(std::thread([](int input_num) {
            for (std::size_t count = 0; count < 100; count++) {
                Logger_AF::Logger::put<Logger_AF::Level::FATAL>("Hello from thread {}, iteration number: {}", input_num,
                                                                count);
                std::cout << " test " << input_num << " " << "count: " << count << std::endl;
            }
            return;
        }, i));
    }

    for (auto &th: threads) {
        th.join();
    }
}
