cmake_minimum_required(VERSION 3.10)

add_executable(logicParser
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cpp
)

target_link_libraries(logicParser PRIVATE customWarning binaryTree Buffer)

target_include_directories(logicParser PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)