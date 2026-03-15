cmake_minimum_required(VERSION 4.1)
project(tachyon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(./src)

add_executable(tachyon 
	src/main.cpp
	src/csv.cpp
	src/order.cpp
	src/order.hpp
	src/csv.hpp
	)

