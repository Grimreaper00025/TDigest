cmake_minimum_required(VERSION 3.10)
project(TDigestProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra")

include_directories(include)

# 1) Build a static library for tdigest core and its concurrent wrapper
add_library(tdigest_lib STATIC
    src/tdigest.cpp
    src/concurrent_tdigest.cpp
)

# 2) data_generator : no link needed
add_executable(data_generator src/data_generator.cpp)

# 3) ground_truth : no link needed
add_executable(ground_truth src/ground_truth.cpp)

# 4) benchmark : needs tdigest_lib
add_executable(benchmark src/benchmark.cpp)
target_link_libraries(benchmark PRIVATE tdigest_lib)

# 5) main streaming app : needs tdigest_lib
add_executable(tdigest_main
    src/main.cpp
)
target_link_libraries(tdigest_main PRIVATE tdigest_lib)
