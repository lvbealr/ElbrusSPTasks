cmake_minimum_required(VERSION 3.10)

add_executable(findSubarray
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(findSubarray PRIVATE customWarning)