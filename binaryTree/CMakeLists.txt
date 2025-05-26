cmake_minimum_required(VERSION 3.10)

add_library(binaryTree INTERFACE)

target_include_directories(binaryTree INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)