set(target test)

addExecutable(${target})
target_link_libraries(${target} PRIVATE OpenCV::OpenCV Calibrate::Chessboard spdlog::spdlog)

if(WIN32)
    copyDll(${target})
endif()
