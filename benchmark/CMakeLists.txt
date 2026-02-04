cmake_minimum_required(VERSION 3.10)
project(benchmark)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(../include)

add_executable(benchmark benchmark.cpp ../source/orderbook.cpp)
