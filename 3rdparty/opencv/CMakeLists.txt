if(WIN32)
    add_library(OpenCVImported SHARED IMPORTED GLOBAL)
    set_target_properties(
        OpenCVImported
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"

        IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world490d.dll"
        IMPORTED_IMPLIB_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv_world490d.lib"

        IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world490.dll"
        IMPORTED_IMPLIB_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv_world490.lib"
        
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    )
    add_library(OpenCV::OpenCV ALIAS OpenCVImported)
else()
    include(${CMAKE_SOURCE_DIR}/cmake/findopencv.cmake)
endif()
