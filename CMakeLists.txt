cmake_minimum_required(VERSION 2.8.4)
project(mailbox)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I ~/boost_1_53_0")

set(SOURCE_FILES main.cpp)
add_executable(mailbox ${SOURCE_FILES})