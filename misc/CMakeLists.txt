cmake_minimum_required(VERSION 3.9)
project(misc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)


find_package(pybind11 REQUIRED)
find_package(GTest REQUIRED)


#----- Google Testing -------------#

enable_testing()
add_executable(gtest-example gtest-example.cpp)
target_link_libraries(gtest-example  GTest::GTest GTest::Main)
add_test(GTestExperiment gtest-example)


add_executable(doctest1 doctest1.cpp)


#----- TinyXML2 --------------------#
# include(tinyxml2_helper.cmake)

add_executable(tinyxml2-test tinyxml2-test.cpp)
target_link_libraries(tinyxml2-test PRIVATE ${tinyxml2_LIBRARY})

