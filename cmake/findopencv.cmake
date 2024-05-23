find_package(OpenCV REQUIRED)
add_library(OpenCVInterFace INTERFACE)
target_include_directories(OpenCVInterFace INTERFACE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(OpenCVInterFace INTERFACE ${OpenCV_LIBS})

add_library(OpenCV::OpenCV ALIAS OpenCVInterFace)
