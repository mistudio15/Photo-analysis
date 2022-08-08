cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)
project(main)

file(GLOB SOURCES "*.cpp", "**/*.cpp")
file(GLOB INCLUDE_DIRS include)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

message("SOURCES = ${SOURCES}")