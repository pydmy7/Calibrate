cmake_minimum_required(VERSION 3.15)

if (MYSELF)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    # set(CMAKE_BUILD_TYPE Release)
    set(CMAKE_BUILD_TYPE Debug)
else()
    set(CMAKE_BUILD_TYPE Release)
endif()

project(
    Calibrate
    LANGUAGES CXX
    DESCRIPTION "Calibrate Library"
    VERSION 0.1.0
)

include(cmake/platform.cmake)
include(cmake/sanitize.cmake)
include(cmake/clangtidy.cmake)
include(cmake/clangformat.cmake)
include(cmake/addtarget.cmake)
include(cmake/copydll.cmake)

add_subdirectory(3rdparty)
add_subdirectory(chessboard)

if (MYSELF)
    include(CTest)
    enable_testing()
    add_subdirectory(unittest)
endif()
