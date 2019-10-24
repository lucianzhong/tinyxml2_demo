cmake_minimum_required(VERSION 3.5)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#project name
PROJECT(xml_test)

add_definitions(-std=c++11 -O3)

set(sources src/tinyxml2.cpp src/xml2_test.cpp)

add_executable(xml_test ${sources})