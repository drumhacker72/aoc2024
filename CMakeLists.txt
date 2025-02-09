cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)

project(aoc2024)

include(FetchContent)
FetchContent_Declare(
    lexy
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/foonathan/lexy/releases/download/v2022.12.1/lexy-src.zip
)
FetchContent_MakeAvailable(lexy)

add_executable(day1 day1.cpp)
target_link_libraries(day1 PRIVATE foonathan::lexy)

add_executable(day2 day2.cpp)
target_link_libraries(day2 PRIVATE foonathan::lexy)

add_executable(day3 day3.cpp)

add_executable(day4 day4.cpp)

add_executable(day5 day5.cpp)
target_link_libraries(day5 PRIVATE foonathan::lexy)

add_executable(day6 day6.cpp)

add_executable(day7 day7.cpp)
target_link_libraries(day7 PRIVATE foonathan::lexy)

add_executable(day8 day8.cpp)

add_executable(day9 day9.cpp)

add_executable(day10 day10.cpp)

add_executable(day11 day11.cpp)

add_executable(day12 day12.cpp)
