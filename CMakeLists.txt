cmake_minimum_required(VERSION 3.15)

if (MYSELF)
    include(cmake/myself.cmake)
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

include(CTest)
enable_testing()
add_subdirectory(unittest)
