cmake_minimum_required(VERSION 3.15)

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
