cmake_minimum_required(VERSION 3.6)
project(CppKafka)

set(CMAKE_CXX_STANDARD 98)

include_directories("tools/lib/librdkafka/include")

add_executable(CppKafka main.cpp simple/producer.h simple/producer.cpp)
