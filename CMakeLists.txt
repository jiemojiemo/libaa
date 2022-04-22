cmake_minimum_required(VERSION 3.10)

project(libaa LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
if (NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
endif ()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(COVERAGE "Use coverage flag" OFF)
option(LIBAA_BUILD_TESTING "Build unit test" ON)
option(LIBAA_INSTALL "install targets" ON)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(LIBAA_BUILD_DOCS "Build docs" OFF)
if (COVERAGE)
    message(STATUS "build with coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif ()

if (ENABLE_ASAN)
    message(STATUS "build with ASAN")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()


set(LIBAA_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBAA_3RD_DIR ${LIBAA_DIR}/third_party)
set(LIBAA_INC_DRI ${LIBAA_DIR}/include)

add_subdirectory(third_party)
add_subdirectory(src)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND LIBAA_BUILD_TESTING)
    add_subdirectory(app)
    add_subdirectory(examples)
    enable_testing()
    add_subdirectory(tests)
endif ()

if (LIBAA_BUILD_DOCS)
    add_subdirectory(docs)
endif ()