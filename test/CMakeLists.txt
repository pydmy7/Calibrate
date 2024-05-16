set(target test)

addExecutable(${target})
target_link_libraries(${target} PRIVATE OpenCV::OpenCV Calibrate::Chessboard)

if(WIN32)
    copyDll(${target})
endif()
