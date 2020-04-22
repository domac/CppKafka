cmake_minimum_required(VERSION 3.6)
project(CppKafka)

set(CMAKE_CXX_STANDARD 98)

include_directories("tools/lib/librdkafka/include")

add_executable(CppKafka main.cpp
        reconnect/producer.h
        reconnect/producer.cpp
        reconnect/consumer.h
        reconnect/consumer.cpp
        base/lock.h
        base/lock.cpp
        base/singleton.h)
