find_package(OpenCV REQUIRED)
add_library(OpenCV::OpenCV INTERFACE)
target_include_directories(OpenCV::OpenCV INTERFACE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(OpenCV::OpenCV INTERFACE ${OpenCV_LIBS})
