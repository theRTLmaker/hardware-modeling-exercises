cmake_minimum_required(VERSION 3.20)
project(hardware_modeling_exercises LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Warnings)

add_subdirectory(1_bounded_queue_with_backpressure)
add_subdirectory(2_discrete_event_simulator)