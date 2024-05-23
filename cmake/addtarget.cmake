macro(addExecutable target)
    aux_source_directory(./src src)
    message("-------${target}_src-------")
    foreach(file ${src})
        message("${file}")
    endforeach()
    message("-------${target}_src-------")

    file(GLOB_RECURSE include ./include/${target}/*.hpp)
    message("-------${target}_include-------")
    foreach(file ${include})
        message("${file}")
    endforeach()
    message("-------${target}_include-------")

    add_executable(
        ${target}
        ${src}
    )

    target_include_directories(${target} PRIVATE include)
    target_compile_features(${target} PRIVATE cxx_std_17)

    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            # target_compile_options(${target} PRIVATE /fsanitize=address)
            # target_link_options(${target} PRIVATE /fsanitize=address)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(${target} PRIVATE -fsanitize=address)
            target_link_options(${target} PRIVATE -fsanitize=address)
        endif()
    endif()

endmacro(addExecutable)


macro(addLibrary target)
    aux_source_directory(./src src)
    message("-------${target}_src-------")
    foreach(file ${src})
        message("${file}")
    endforeach()
    message("-------${target}_src-------")

    file(GLOB_RECURSE include ./include/${target}/*.hpp)
    message("-------${target}_include-------")
    foreach(file ${include})
        message("${file}")
    endforeach()
    message("-------${target}_include-------")

    add_library(
        ${target}
        STATIC
        ${src}
    )

    target_include_directories(${target} PUBLIC include)
    target_compile_features(${target} PRIVATE cxx_std_17)

    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            # target_compile_options(${target} PRIVATE /fsanitize=address)
            # target_link_options(${target} PRIVATE /fsanitize=address)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(${target} PRIVATE -fsanitize=address)
            target_link_options(${target} PRIVATE -fsanitize=address)
        endif()
    endif()

endmacro(addLibrary)
