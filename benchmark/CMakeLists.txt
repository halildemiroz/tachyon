cmake_minimum_required(VERSION 4.1)
project(tachyon)

set(CMAKE_CXX_STANDARD 20)          
set(CMAKE_CXX_STANDARD_REQUIRED ON)   
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(../src)

add_executable(benchmark 
	./main.cpp
	../src/order.cpp
	../src/csv.cpp
	../src/csv.hpp
	../src/order.hpp
	../src/logger.cpp
	../src/logger.hpp
	)
