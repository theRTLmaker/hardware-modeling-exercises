add_library(kata01_bounded_queue INTERFACE)
target_include_directories(kata01_bounded_queue INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(test_bounded_queue
  tests/test_bounded_queue.cpp
)
target_link_libraries(test_bounded_queue PRIVATE kata01_bounded_queue)

add_test(NAME kata01_bounded_queue COMMAND test_bounded_queue)