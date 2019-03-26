cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(easy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT -g -Wall -std=c++11 -rdynamic")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

include_directories(${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/3rd)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp ${PROJECT_SOURCE_DIR}test/*.h)

file(GLOB_RECURSE LIB_FILES ${PROJECT_SOURCE_DIR}/lib/*.cpp ${PROJECT_SOURCE_DIR}/lib/*.h ${PROJECT_SOURCE_DIR}/3rd/*.cpp)

add_library(easy STATIC ${LIB_FILES})

add_executable(test_lib ${SRC_FILES})

target_link_libraries(test_lib easy pthread)

add_subdirectory(tools)