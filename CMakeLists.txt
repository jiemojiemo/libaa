cmake_minimum_required(VERSION 3.10)

project(libaa LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

option(COVERAGE "Use coverage flag" OFF)
option(BUILD_TESTING "Build unit test" ON)
option(LIBAA_INSTALL "install targets" ON)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if(COVERAGE)
    message(STATUS "build with coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

if(ENABLE_ASAN)
    message(STATUS "build with ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()


set(LIBAA_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBAA_3RD_DIR ${LIBAA_DIR}/third_party)
set(LIBAA_INC_DRI ${LIBAA_DIR}/include)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(examples)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
