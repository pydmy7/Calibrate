if(WIN32)
    add_library(OpenCVImported SHARED IMPORTED GLOBAL)
    set_target_properties(
        OpenCVImported
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
        IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world490d.dll"
        IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv_world490d.lib"
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    )
    add_library(OpenCV::OpenCV ALIAS OpenCVImported)
else()
    include(${CMAKE_SOURCE_DIR}/cmake/findopencv.cmake)
endif()
