cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

project(aoc2024)

include(FetchContent)

FetchContent_Declare(
    lexy
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/foonathan/lexy/releases/download/v2022.12.1/lexy-src.zip
)
FetchContent_MakeAvailable(lexy)

FetchContent_Declare(
    PixelToaster
    GIT_REPOSITORY https://github.com/thedmd/pixeltoaster.git
    GIT_TAG c791826964cde9465fa113a5e15c494aa33f059b
)
FetchContent_MakeAvailable(PixelToaster)

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

add_executable(day13 day13.cpp)
target_link_libraries(day13 PRIVATE foonathan::lexy)

add_executable(day14 day14.cpp)
target_include_directories(day14 PRIVATE ${PixelToaster_SOURCE_DIR})
target_link_libraries(day14 PRIVATE foonathan::lexy PixelToaster)

add_executable(day15 day15.cpp)

add_executable(day16 day16.cpp)

add_executable(day17 day17.cpp)
target_link_libraries(day17 PRIVATE foonathan::lexy)

add_executable(day18 day18.cpp)

add_executable(day19 day19.cpp)

add_executable(day20 day20.cpp)

add_executable(day21 day21.cpp)

add_executable(day22 day22.cpp)

add_executable(day23 day23.cpp)
