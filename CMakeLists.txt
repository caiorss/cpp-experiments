cmake_minimum_required(VERSION 3.9)
project(cppexperiments)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(FORCE_CLANG OFF)

# Force compiler to Clang++
if(FORCE_CLANG)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
endif()


message( [COMPILER] "Compiler = ${CMAKE_CXX_COMPILER}" )


message(" [INFO]  System Name  - CMAKE_SYSTEM_NAME     = ${CMAKE_SYSTEM_NAME}")
message(" [INFO]  Build type   - CMAKE_BUILD_TYPE      = ${CMAKE_BUILD_TYPE}")
message(" [INFO]  Compiler ID  - CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message(" [INFO]  Install path - CMAKE_INSTALL_PREFIX  = ${CMAKE_INSTALL_PREFIX} ")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
   message(" [INFO] Target = 64 bits build.")
else()
   message(" [INFO] Target = 32 bits build.")
endif()
message(" [INFO] CMake binary dir = ${CMAKE_BINARY_DIR}")

execute_process(COMMAND  echo " [INFO] PATH = $\{which conan\}" )


# Copy target file to current directory whenerver it is rebuilt
function(copy_after_build TARGET_NAME )
    # Note: CMAKE_CURRENT_LIST_DIR is the directory where is this
    # CMakeLists.txt file.
    set(DESTDIR ${CMAKE_CURRENT_LIST_DIR}/bin/)
    file(MAKE_DIRECTORY ${DESTDIR})

    # Copy binary file to <CMakeLists.txt didctory>./bin
    # after target is compiled.
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
                               $<TARGET_FILE:${TARGET_NAME}> ${DESTDIR}
                               )
endfunction()


# ============= Conan Boosttrap =============================#

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.13/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

# Possible values "default" and "llvm8"
set(CONAN_PROFILE default)

conan_cmake_run(REQUIRES
                  range-v3/0.5.0@ericniebler/stable
                  fmt/5.3.0@bincrafters/stable
                  doctest/2.3.1@bincrafters/stable
                  pybind11/2.2.4@conan/stable
                  exprtk/20181202@bincrafters/stable
                  gtest/1.8.1@bincrafters/stable

                  # Parser combinator library
                  pegtl/2.8.0@taocpp/stable

                  tinyxml2/7.0.1@nicolastagliani/stable

                  # Note: Add this repository for usign DLIB
                  # $ conan remote add omaralvarez https://api.bintray.com/conan/omaralvarez/public-conan
                  #
                  # https://bintray.com/omaralvarez/public-conan/dlib%3Aomaralvarez/19.16.0%3Astable
                  # https://github.com/omaralvarez/conan_dlib
                  # dlib/19.16.0@omaralvarez/stable

                # PROFILE ${CONAN_PROFILE}
                BASIC_SETUP
                BUILD missing)

find_package(Boost REQUIRED COMPONENTS system context)
find_package(pybind11 REQUIRED)
find_package(GTest REQUIRED)
# find_package(Tinyxml2_static REQUIRED)


 # ===== TinyXML 2 ===========================================#
# set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")
# find_package(TinyXML2 REQUIRED)

 #======= Targets Settings ===============+#



set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(cotire)

add_executable(rangesv3  misc/ranges-v3.cpp)
#target_link_libraries(agtest PRIVATE ${CONAN_LIBS})

add_executable(fmt-experiments misc/fmt-experiments.cpp)

# target_link_libraries(fmt-experiments PRIVATE ${CONAN_LIBS})

add_executable(doctest1 misc/doctest1.cpp)

#----- TinyXML2 --------------------#


add_executable(tinyxml2-test misc/tinyxml2-test.cpp)
target_link_libraries(tinyxml2-test PRIVATE ${tinyxml2_LIBRARY})

#----- Google Testing -------------#

enable_testing()
add_executable(gtest-example gtest-example.cpp)
target_link_libraries(gtest-example  GTest::GTest GTest::Main)
add_test(GTestExperiment gtest-example)

#---- Lua lua_c_api -------------------#

add_executable(lua_c_api lua_c_api.cpp)
target_link_libraries(lua_c_api PRIVATE lua-5.3)
copy_after_build(lua_c_api)

#----- Lua Sol2 -------------------#

add_executable(luaSol1  lua-sol1.cpp)
target_link_libraries(luaSol1 PRIVATE  lua-5.3)
copy_after_build(luaSol1)

#---- PEGTL - Parser Combinator -----#

add_executable(pgetl1-parser pgetl1-parser.cpp)

#---- Pybind11 - Pyembed ------------#

add_executable(pyembed pyembed.cpp)
target_link_libraries(pyembed PRIVATE pybind11::embed)
copy_after_build(pyembed)

configure_file(
  "pyembed_script.py"
  "${CMAKE_CURRENT_BINARY_DIR}/bin" COPYONLY
)

if(false)
  add_executable(exprtk1 misc/exprtk1.cpp)
  target_link_libraries(exprtk1 PRIVATE ${CONAN_LIBS})
  cotire(exprtk1)
endif()

add_executable(asio_server1 asio/asio_server1.cpp)
target_link_libraries(asio_server1 PRIVATE Boost::system pthread)

add_executable(asio_server2 asio/asio_server2.cpp)
target_link_libraries(asio_server2 PRIVATE Boost::system pthread)
copy_after_build(asio_server2)


add_executable(asio_echo asio/asio_echo.cpp)
target_link_libraries(asio_echo PRIVATE Boost::system pthread)
copy_after_build(asio_echo)


add_executable(asio_basic1 asio/asio_basic1.cpp)
target_link_libraries(asio_basic1 PRIVATE Boost::system pthread)
copy_after_build(asio_basic1)

if(false)
 add_executable(asio_stream_server asio/asio_stream_server.cpp)
 target_link_libraries(asio_stream_server PRIVATE Boost::system pthread)
 copy_after_build(asio_stream_server)
endif()


add_executable(http_client_sync1 asio/http_client_sync1.cpp)
target_link_libraries(http_client_sync1 PRIVATE Boost::system pthread)
copy_after_build(http_client_sync1)



add_executable(http_client_async1 asio/http_client_async1.cpp)
target_link_libraries(http_client_async1 PRIVATE Boost::system pthread)
cotire(http_client_async1)
copy_after_build(http_client_async1)

add_executable(coroutine1 coroutine1.cpp)
copy_after_build(coroutine1)
target_link_libraries(coroutine1 PRIVATE Boost::context)


message([COMPILER] ${CMAKE_CXX_COMPILER_ID})

if(${CMAKE_CXX_COMPILER_ID} MATCHES CLANG)
  add_executable(cpp20-coroutine1 cpp20/cpp20-coroutine1.cpp)
  target_compile_features(cpp20-coroutine1 PRIVATE cxx_std_20)
  target_compile_options(cpp20-coroutine1 PRIVATE -fcoroutines-ts -stdlib=libc++ )
  target_link_options(cpp20-coroutine1 PUBLIC  -stdlib=libc++)
endif()

# add_executable(pimpl pimpl/main.cpp pimpl/robot.cpp)
# target_include_directories(pimpl PRIVATE ./pimpl)



