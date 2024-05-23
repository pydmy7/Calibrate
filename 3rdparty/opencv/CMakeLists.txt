if(WIN32)
    # Expand-Archive -Path ".\opencv_world490.zip" -DestinationPath "./"

    execute_process(
        COMMAND pwsh -Command "Expand-Archive -Path '${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world490.zip' -DestinationPath '${CMAKE_CURRENT_SOURCE_DIR}/bin/'"
        COMMAND pwsh -Command "Expand-Archive -Path '${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world490d.zip' -DestinationPath '${CMAKE_CURRENT_SOURCE_DIR}/bin/'"
        COMMAND pwsh -Command "Expand-Archive -Path '${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv_world490.zip' -DestinationPath '${CMAKE_CURRENT_SOURCE_DIR}/lib/'"
        COMMAND pwsh -Command "Expand-Archive -Path '${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv_world490d.zip' -DestinationPath '${CMAKE_CURRENT_SOURCE_DIR}/lib/'"
    )

    add_library(OpenCV::OpenCV SHARED IMPORTED GLOBAL)
    set_target_properties(
        OpenCV::OpenCV
        PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"

        IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world490d.dll"
        IMPORTED_IMPLIB_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv_world490d.lib"

        IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/opencv_world490.dll"
        IMPORTED_IMPLIB_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv_world490.lib"
        
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
        IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
    )
else()
    include(${CMAKE_SOURCE_DIR}/cmake/findopencv.cmake)
endif()
