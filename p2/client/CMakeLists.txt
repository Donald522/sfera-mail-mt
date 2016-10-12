cmake_minimum_required(VERSION 2.6)
project(ChatClient)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Client.cpp Client.h)
add_executable(ChatClient ${SOURCE_FILES})