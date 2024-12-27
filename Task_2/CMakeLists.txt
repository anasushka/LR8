cmake_minimum_required(VERSION 3.29)
project(Task_2)

set(CMAKE_CXX_STANDARD 23)
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

add_executable(Task_2 main.cpp
         file.cpp
        checks.cpp
        funelastic.cpp
        fun.cpp
        header.h
        hehe.h
        file.cpp
)

target_link_libraries(Task_2 curl jsoncpp)