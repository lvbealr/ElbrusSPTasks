cmake_minimum_required(VERSION 3.10)

add_executable(processSyscall
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IO.cpp
)

target_link_libraries(processSyscall PRIVATE customWarning)

target_include_directories(processSyscall PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(processSyscall PRIVATE _GNU_SOURCE _BSD_SOURCE)