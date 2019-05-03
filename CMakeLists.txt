cmake_minimum_required(VERSION 3.9)
project(cppexperiments)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(FORCE_CLANG ON)

# Force compiler to Clang++
if(FORCE_CLANG)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
endif()


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


#configure_file(${CMAKE_)

if(false)
add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake"
    DEPENDS ${CMAKE_CURRENT_LIST_DIR}/conanfile.txt
    COMMAND /home/archbox/opt/anaconda3/bin/conan install . --install-folder "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
endif()


message(" [TRACE] Before Creating conan file.")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    message(" [TRACE] Creating conan file ...")
  execute_process(
    COMMAND           conan install . --install-folder "${CMAKE_BINARY_DIR}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
   )
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


               #======= Targets Settings ===============+#

add_executable(rangesv3 "ranges-v3.cpp")
#target_link_libraries(agtest PRIVATE ${CONAN_LIBS})

add_executable(fmt-experiments fmt-experiments.cpp)
target_link_libraries(fmt-experiments PRIVATE ${CONAN_LIBS})

add_executable(doctest1 doctest1.cpp)
target_link_libraries(doctest1 PRIVATE ${CONAN_LIBS})


add_executable(exprtk1 exprtk1.cpp)
target_link_libraries(exprtk1 PRIVATE ${CONAN_LIBS})


