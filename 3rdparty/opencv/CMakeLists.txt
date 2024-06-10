if(WIN32)
    add_library(OpenCV::OpenCV SHARED IMPORTED GLOBAL)
    set_target_properties(
        OpenCV::OpenCV
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"

        IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world4100d.dll"
        IMPORTED_IMPLIB_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world4100d.lib"

        IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world4100.dll"
        IMPORTED_IMPLIB_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world4100.lib"
        
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    )
else()
    include(${PROJECT_SOURCE_DIR}/cmake/findopencv.cmake)
endif()
