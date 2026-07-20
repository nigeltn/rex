cmake_minimum_required(VERSION 3.24)
project(rex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(rex src/parser.cpp)

target_include_directories(rex PUBLIC include)

target_compile_options(rex PRIVATE -Wall -Wextra -Werror)

target_compile_options(rex PUBLIC $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
target_link_options(rex PUBLIC $<$<CONFIG:Debug>:-fsanitize=address,undefined>)

# ---- tests ----
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(rex_tests tests/test_parser.cpp)
target_link_libraries(rex_tests PRIVATE rex GTest::gtest_main)
target_compile_options(rex_tests PRIVATE -Wall -Wextra -Werror)

include(GoogleTest)
gtest_discover_tests(rex_tests)
