cmake_minimum_required(VERSION 3.25)
project(aoc2023)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS REQUIRED Core)

add_subdirectory(day01)
add_subdirectory(day02)
