cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER clang++)

project(
    Calibrate
    LANGUAGES CXX
    DESCRIPTION "Calibrate Library"
    VERSION 0.1.0
)

include(cmake/addtarget.cmake)
include(cmake/copydll.cmake)
include(cmake/platform.cmake)

add_subdirectory(3rdparty)
add_subdirectory(chessboard)
add_subdirectory(test)
