set(target chessboard)

addLibrary(${target})
target_link_libraries(${target} PRIVATE OpenCV::OpenCV)

add_library(Calibrate::Chessboard ALIAS ${target})
