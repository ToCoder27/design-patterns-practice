cmake_minimum_required(VERSION 3.10)
project(strategy)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(strategy strategy.cpp)
add_executable(my_strategy my_strategy.cpp)
add_executable(my_strategy_plus my_strategy_plus.cpp)
