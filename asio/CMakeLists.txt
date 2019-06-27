cmake_minimum_required(VERSION 3.9)
project(boost_asio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)



find_package(Boost REQUIRED COMPONENTS system context)

#================= Targets ========================#

add_executable(asio_server1 asio_server1.cpp)
target_link_libraries(asio_server1 PRIVATE Boost::system pthread)

add_executable(asio_server2 asio_server2.cpp)
target_link_libraries(asio_server2 PRIVATE Boost::system pthread)
# copy_after_build(asio_server2)


add_executable(asio_echo asio_echo.cpp)
target_link_libraries(asio_echo PRIVATE Boost::system pthread)
# copy_after_build(asio_echo)


add_executable(asio_basic1 asio_basic1.cpp)
target_link_libraries(asio_basic1 PRIVATE Boost::system pthread)
# copy_after_build(asio_basic1)

if(false)
 add_executable(asio_stream_server asio_stream_server.cpp)
 target_link_libraries(asio_stream_server PRIVATE Boost::system pthread)
 copy_after_build(asio_stream_server)
endif()


add_executable(http_client_sync1 http_client_sync1.cpp)
target_link_libraries(http_client_sync1 PRIVATE Boost::system pthread)
# copy_after_build(http_client_sync1)



add_executable(http_client_async1 http_client_async1.cpp)
target_link_libraries(http_client_async1 PRIVATE Boost::system pthread)
# cotire(http_client_async1)
#copy_after_build(http_client_async1)

