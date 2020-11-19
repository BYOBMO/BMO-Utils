cmake_minimum_required(VERSION 3.1)
project(arms)

# Define executable target
include_directories(${CMAKE_BINARY_DIR})

add_executable(arms arms.c)
target_link_libraries(arms wiringPi wiringPiDev pthread m wiringPiPca9685)

