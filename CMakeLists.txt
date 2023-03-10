project(ccontain)

cmake_minimum_required(VERSION 3.12.0)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

set(VEC_INCLUDE ${SRC_DIR}/vec/include)
set(COMMON_INCLUDE ${SRC_DIR}/common/include)

add_subdirectory(src)
add_subdirectory(test)
