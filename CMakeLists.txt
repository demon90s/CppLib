cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp ${PROJECT_SOURCE_DIR}test/*.h)

file(GLOB_RECURSE LIB_FILES ${PROJECT_SOURCE_DIR}/lib/*.cpp ${PROJECT_SOURCE_DIR}/lib/*.h)

add_library(easy STATIC ${LIB_FILES})

add_executable(main ${SRC_FILES})

target_link_libraries(main easy pthread)