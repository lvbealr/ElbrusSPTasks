project(sortArrayLib)
cmake_minimum_required(VERSION 3.10)

add_library(sortArrayLib STATIC 
    src/sort.cpp
    src/sortContext.cpp
    main.cpp
)

target_link_libraries(sortArrayLib PUBLIC customWarning)

target_include_directories(sortArrayLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(parallelSort main.cpp)

target_link_libraries(parallelSort PRIVATE sortArrayLib)