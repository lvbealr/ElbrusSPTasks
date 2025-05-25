cmake_minimum_required(VERSION 3.10)

add_executable(forkFile
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fork.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IO.cpp
)

target_link_libraries(forkFile PRIVATE customWarning)

target_include_directories(forkFile PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)