cmake_minimum_required(VERSION 3.10)
project(processSyscall LANGUAGES CXX)

set(DEBUG_FLAGS "-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations                                     \
							 -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported     \
							 -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security				\
							 -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual     \
							 -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo 				\
							 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods 				\
							 -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef 					\
							 -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix 								\
							 -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new  \
							 -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer        \
							 -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,    \
							 float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,       \
							 return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
set(RELEASE_FLAGS -O3 -DNDEBUG -std=c++17)

get_property(CURRENT_CONFIG GLOBAL PROPERTY CONFIG)

if (CURRENT_CONFIG STREQUAL "Debug")
    add_compile_options(${DEBUG_FLAGS})
elseif(CURRENT_CONFIG STREQUAL "Release")
    add_compile_options(${RELEASE_FLAGS})
endif()

add_subdirectory(customWarning)
add_subdirectory(syscall)