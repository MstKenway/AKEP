cmake_minimum_required(VERSION 3.10)
project(AKEP C)

set(CMAKE_C_STANDARD 11)

#add_definitions("-Wall -g")
add_subdirectory(AKEP-src)
#md5库
add_subdirectory(WjCryptLib)

include_directories(AKEP-src/include)
include_directories(WjCryptLib/lib)

add_executable(AKEP main.c)
add_executable(TEST test.c AKEP-src/src/AKEP_DATA_STRUCT.c AKEP-src/src/AKEP_Base.c)


target_link_libraries(AKEP AKEP_SRC)
target_link_libraries(AKEP WjCryptLib)

target_link_libraries(TEST WjCryptLib)

