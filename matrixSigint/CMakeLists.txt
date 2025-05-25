cmake_minimum_required(VERSION 3.10)

add_executable(sigint
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/interruption.cpp
)

target_link_libraries(sigint PRIVATE customWarning)

target_include_directories(sigint PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(sigint PRIVATE -mavx2)