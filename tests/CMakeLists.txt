cmake_minimum_required(VERSION 4.1)
project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(../src)

add_executable(test 
	../src/csv.cpp
	../src/order.cpp
	main.cpp)
