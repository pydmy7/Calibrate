cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-Wall -Wextra -Wpedantic)

project(
    Calibrate
    LANGUAGES CXX C
    DESCRIPTION "Calibrate Tools"
    VERSION 0.1.0
)

include(cmake/addtarget.cmake)
include(cmake/copydll.cmake)
include(cmake/platform.cmake)

add_subdirectory(3rdparty)
add_subdirectory(chessboard)
add_subdirectory(test)
