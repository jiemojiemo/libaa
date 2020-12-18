cmake_minimum_required(VERSION 3.10)

project(libaa LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")


# must return value if you define non-void function
add_compile_options(-Werror=return-type)

# warning as error
add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-unused-function -Wno-unused-parameter -Wno-missing-braces )


include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

option(COVERAGE "Use coverage flag" OFF)
option(BUILD_TESTING "Build unit test" ON)

if(COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
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
