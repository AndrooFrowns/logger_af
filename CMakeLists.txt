cmake_minimum_required(VERSION 3.20)
project(logger_af)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -pthread)

# get rid of this once std::format() is implemented by at least gcc
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
        )
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master
        )
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(logger_af src/logger_af.h)
target_link_libraries(logger_af
        PRIVATE
        fmt::fmt
        )

enable_testing()

add_executable(main src/main.cpp src/logger_af.h)
target_link_libraries(main PRIVATE fmt::fmt)

add_executable(hello_test test/hello_test.cpp)
target_link_libraries(hello_test PRIVATE fmt::fmt logger_af gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)


