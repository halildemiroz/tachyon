cmake_minimum_required(VERSION 3.10)
project(tachyon)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tachyon
	source/main.cpp
	source/lob.cpp
	)

