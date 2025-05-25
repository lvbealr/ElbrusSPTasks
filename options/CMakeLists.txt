cmake_minimum_required(VERSION 3.10)

add_executable(parseCommandLine
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
)

target_link_libraries(parseCommandLine PRIVATE customWarning)

target_include_directories(parseCommandLine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)