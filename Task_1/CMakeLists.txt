cmake_minimum_required(VERSION 3.29)
project(Task_1)

set(CMAKE_CXX_STANDARD 23)
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(Task_1 main.cpp
        fun1.cpp
        funelastic.cpp
        hehe.h
        file.cpp
        checks.cpp
        header.h
)

target_link_libraries(Task_1 curl jsoncpp)