cmake_minimum_required(VERSION 3.10)

add_executable(fileInfo
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/getterInfo.cpp
)

target_link_libraries(fileInfo PRIVATE customWarning)

target_include_directories(fileInfo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)