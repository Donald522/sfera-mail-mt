cmake_minimum_required(VERSION 2.6)
project(ChatServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Server.cpp Server.h)
add_executable(ChatServer ${SOURCE_FILES})