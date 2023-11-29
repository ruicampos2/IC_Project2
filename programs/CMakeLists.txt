cmake_minimum_required(VERSION 3.16)
project(opencv-example)

find_package(OpenCV REQUIRED)

SET (CMAKE_BUILD_TYPE "Release")
#SET (CMAKE_BUILD_TYPE "Debug")

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++17 -pthread")
SET (CMAKE_CXX_FLAGS_RELEASE "-O3")
SET (CMAKE_CXX_FLAGS_DEBUG "-g3 -fsanitize=address")

SET (BASE_DIR ${CMAKE_SOURCE_DIR} )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../opencv-bin)

add_executable (program1 program1.cpp)
target_link_libraries (program1 ${OpenCV_LIBS})

add_executable (program2a program2a.cpp)
target_link_libraries (program2a ${OpenCV_LIBS})

add_executable (program2b program2b.cpp)
target_link_libraries (program2b ${OpenCV_LIBS})

add_executable (program2c program2c.cpp)
target_link_libraries (program2c ${OpenCV_LIBS})

add_executable (program2d program2d.cpp)
target_link_libraries (program2d ${OpenCV_LIBS})

add_executable (program4dec program4dec.cpp)
target_link_libraries (program4dec sndfile fftw3)

add_executable (program4enc program4enc.cpp)
target_link_libraries (program4enc sndfile fftw3)
