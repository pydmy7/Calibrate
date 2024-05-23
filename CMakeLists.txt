cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

project(
    Calibrate
    LANGUAGES CXX C
    DESCRIPTION "Calibrate Library"
    VERSION 0.1.0
)

include(cmake/platform.cmake)
include(cmake/sanitize.cmake)
include(cmake/addtarget.cmake)
include(cmake/copydll.cmake)

add_subdirectory(3rdparty)
add_subdirectory(chessboard)
add_subdirectory(test)
