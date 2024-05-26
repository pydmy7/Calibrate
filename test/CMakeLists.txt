set(target test)

addExecutable(${target})
target_link_libraries(${target} PRIVATE OpenCV::OpenCV Calibrate::Chessboard spdlog::spdlog)
target_compile_definitions(${target} PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

copyDll(${target})
