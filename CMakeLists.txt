cmake_minimum_required(VERSION 3.10)

project(slib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB slibsrc ./src/**.cpp)

add_library(${PROJECT_NAME} STATIC ${slibsrc})
target_include_directories(${PROJECT_NAME} PUBLIC ./include)