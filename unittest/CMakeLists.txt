set(target unittest)

addExecutable(${target})
target_link_libraries(${target} PRIVATE OpenCV::OpenCV Calibrate::Chessboard spdlog::spdlog)
copyDll(${target})

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

add_test(NAME "chessboard${target}" COMMAND $<TARGET_FILE:${target}>)
