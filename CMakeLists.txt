cmake_minimum_required(VERSION 3.10)
project(tachyon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include/)

add_executable(tachyon
	source/main.cpp
	source/orderbook.cpp
	include/order.h
	include/orderbook.h
	)

