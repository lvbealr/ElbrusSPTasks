cmake_minimum_required(VERSION 3.10)

add_executable(sortStrings
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sort.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/textStruct.cpp
)

target_link_libraries(sortStrings PRIVATE customWarning)

target_include_directories(sortStrings PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)